#pragma once

#include "api/networking/IServer.h"
#include "api/networking/PacketTypeRegistry.h"

namespace Chimp {
	IServer::IServer(const ConnectionInfo& serverInfo) :
		m_ServerInfo(serverInfo),
		m_EventPollingThread([this]() {
		while (!m_IsBeingDestroyed && !m_HostingFailed) {
			AsyncUpdate();
		}
			}) {

	}

	IServer::~IServer()
	{
		assert(m_IsBeingDestroyed);
	}

	void IServer::SendPacketToClient(int clientId, const NetworkPacket& packet, int channel)
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

	void IServer::SendPacketToAllClients(const NetworkPacket& packet, int channel)
	{
		const char* packetData = (char*)&packet;
		const size_t packetSize = PacketTypeRegistry::GetPacketSize(packet.PacketType);
		char* packetBuffer = new char[packetSize]; // this is deleted by the network packet unique ptr
		memcpy(packetBuffer, packetData, packetSize);
		m_QueuedPacketsToSend.Push([this, channel, packetSize, packetBuffer]() {
			std::unique_ptr<NetworkPacket> packetCopy = PacketTypeRegistry::Parse(packetSize, packetBuffer);
			ImplSendPacketToAllClients(*packetCopy, channel);
			});
	}

	void IServer::SendPacketToAllClientsExcept(const NetworkPacket& packet, const std::vector<int>& excludedClients, int channel)
	{
		const char* packetData = (char*)&packet;
		const size_t packetSize = PacketTypeRegistry::GetPacketSize(packet.PacketType);
		char* packetBuffer = new char[packetSize]; // this is deleted by the network packet unique ptr
		memcpy(packetBuffer, packetData, packetSize);
		m_QueuedPacketsToSend.Push([this, excludedClients, channel, packetSize, packetBuffer]() {
			std::unique_ptr<NetworkPacket> packetCopy = PacketTypeRegistry::Parse(packetSize, packetBuffer);
			ImplSendPacketToAllClientsExcept(*packetCopy, excludedClients, channel);
			});
	}

	void IServer::SendPacketToClientWithResponse(int clientId, const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel)
	{
		const char* packetData = (char*)&packet;
		const size_t packetSize = PacketTypeRegistry::GetPacketSize(packet.PacketType);
		char* packetBuffer = new char[packetSize]; // this is deleted by the network packet unique ptr
		memcpy(packetBuffer, packetData, packetSize);
		m_QueuedPacketsToSend.Push([this, clientId, callback, channel, packetSize, packetBuffer]() {
			std::unique_ptr<NetworkPacket> packetCopy = PacketTypeRegistry::Parse(packetSize, packetBuffer);
			ImplSendPacketToClientWithResponse(clientId, *packetCopy, callback, channel);
			});
	}

	void IServer::SendPacketToSelf(const NetworkPacket& packet)
	{
		const char* packetData = (char*)&packet;
		const size_t packetSize = PacketTypeRegistry::GetPacketSize(packet.PacketType);
		char* packetBuffer = new char[packetSize]; // this is deleted by the network packet unique ptr
		memcpy(packetBuffer, packetData, packetSize);
		std::unique_ptr<NetworkPacket> packetCopy = PacketTypeRegistry::Parse(packetSize, packetBuffer);
		m_EventQueue.Push(std::make_tuple(packet.PacketType, std::move(packetCopy)));
	}

	void IServer::Update()
	{
		m_EventQueue.PopAll([this](const std::tuple<NetworkPacketType, std::shared_ptr<NetworkPacket>>& event) {
			m_EventHandlerAndBroadcaster.Broadcaster->Broadcast(std::get<0>(event), *std::get<1>(event));
			});
		m_SendQueuedPackets = true;
	}

	void IServer::ShutdownThreads()
	{
		m_IsBeingDestroyed = true;
		m_EventPollingThread.join();
	}
}