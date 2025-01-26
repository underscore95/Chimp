#pragma once

#include "api/networking/IClient.h"
#include "api/networking/PacketTypeRegistry.h"

namespace Chimp {
	IClient::IClient(const ConnectionInfo& serverInfo) :
		m_ServerInfo(serverInfo),
		m_EventPollingThread([this]() {
		while (!m_IsBeingDestroyed && !m_FailedToConnect && !m_WasDisconnected) {
			AsyncUpdate();
		}
			}) {

	}

	IClient::~IClient()
	{
		assert(m_IsBeingDestroyed);
	}

	void IClient::Update()
	{
		m_EventQueue.PopAll([this](const std::tuple<NetworkPacketType, std::shared_ptr<NetworkPacket>>& event) {
			auto eventType = std::get<0>(event);
			auto& eventPacket = *std::get<1>(event);
			m_EventHandlerAndBroadcaster.Broadcaster->Broadcast(eventType, eventPacket);
			});
		m_SendQueuedPackets = true;
	}

	// Need to copy the packet so we can send it later
	// this code is scuffed af
	void IClient::SendPacketToServer(const NetworkPacket& packet, int channel)
	{
		const char* packetData = (char*)&packet;
		const size_t packetSize = PacketTypeRegistry::GetPacketSize(packet.PacketType);
		char* packetBuffer = new char[packetSize]; // this is deleted by the network packet unique ptr
		memcpy(packetBuffer, packetData, packetSize);
		m_QueuedPacketsToSend.Push([this, channel, packetSize, packetBuffer]() {
			std::unique_ptr<NetworkPacket> packetCopy = PacketTypeRegistry::Parse(packetSize, packetBuffer);
			ImplSendPacketToServer(*packetCopy, channel);
			});
	}

	void IClient::SendPacketToServerWithResponse(const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel)
	{
		const char* packetData = (char*) &packet;
		const size_t packetSize = PacketTypeRegistry::GetPacketSize(packet.PacketType);
		char* packetBuffer = new char[packetSize]; // this is deleted by the network packet unique ptr
		memcpy(packetBuffer, packetData, packetSize);
		m_QueuedPacketsToSend.Push([this, callback, channel, packetSize, packetBuffer]() {
			std::unique_ptr<NetworkPacket> packetCopy = PacketTypeRegistry::Parse(packetSize, packetBuffer);
			const TestPacket* testPacket2 = static_cast<const TestPacket*>(packetCopy.get());
			ImplSendPacketToServerWithResponse(*packetCopy, callback, channel);
			});
	}

	void IClient::SendPacketToClient(unsigned int clientId, const NetworkPacket& packet, int channel)
	{
		const char* packetData = (char*)&packet;
		const size_t packetSize = PacketTypeRegistry::GetPacketSize(packet.PacketType);
		char* packetBuffer = new char[packetSize]; // this is deleted by the network packet unique ptr
		memcpy(packetBuffer, packetData, packetSize);
		m_QueuedPacketsToSend.Push([this, clientId, channel, packetSize, packetBuffer]() {
			std::unique_ptr<NetworkPacket> packetCopy = PacketTypeRegistry::Parse(packetSize, packetBuffer);
			ImplSendPacketToClient(clientId, *packetCopy, channel);
			});
	}

	bool IClient::WasDisconnected() const
	{
		return m_WasDisconnected;
	}

	void IClient::ImplSendPacketToClient(unsigned int clientId, const NetworkPacket& packet, int channel)
	{
		// We'll send a packet to the server telling the server to forward the next packet it receives okay sounds good
		ToServerForwardPacket forwardPacket;
		forwardPacket.PacketType = Packets::FORWARD;
		forwardPacket.ClientId = clientId;

		ImplSendPacketToServer(forwardPacket, channel);
		ImplSendPacketToServer(packet, channel);
	}

	void IClient::ShutdownThreads()
	{
		m_IsBeingDestroyed = true;
		m_EventPollingThread.join();
	}
}