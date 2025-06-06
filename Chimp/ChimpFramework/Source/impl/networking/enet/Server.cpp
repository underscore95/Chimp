#include "Server.h"
#include "networking/PacketTypeRegistry.h"
#include "Loggers.h"

namespace Chimp {
	Server::Server(const ConnectionInfo& serverInfo)
		: IServer(serverInfo)
	{
		InitENet::Init();

		ENetAddress address = {};
		enet_address_set_host(&address, serverInfo.HostName.c_str());
		address.port = serverInfo.Port;


		m_Server = enet_host_create(
				&address,
				serverInfo.MaxClients,
				serverInfo.MaxChannels,
				serverInfo.MaxIncomingBandwidth,
				serverInfo.MaxOutgoingBandwidth
			);

		if (!m_Server) {
			Loggers::Network().Error("Failed to host ENet server.");
			m_HostingFailed = true;
			return;
		}

		Loggers::Network().Info("Hosting server.");
		m_IsValid = true;
	}

	Server::~Server()
	{
		ShutdownThreads();
		if (m_Server)
		{
			enet_host_destroy(m_Server);
			m_Server = nullptr;
		}
	}

	bool Server::IsValid() const
	{
		return m_IsValid;
	}

	void Server::ImplSendPacketToClient(int clientId, const NetworkPacket& packet, int channel)
	{
		assert(IsValid());

		ENetPeer* peer = m_ClientIdsReverse.at(clientId);
		assert(peer);

		const auto packetSize = PacketTypeRegistry::GetPacketSize(packet.PacketType);

		ENetPacket* enetPacket = enet_packet_create(&packet, packetSize, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, channel, enetPacket);
	}

	void Server::ImplSendPacketToAllClients(const NetworkPacket& packet, int channel)
	{
		for (auto& [peer, clientId] : m_ClientIds)
		{
			ImplSendPacketToClient(clientId, packet, channel);
		}
	}

	void Server::ImplSendPacketToAllClientsExcept(const NetworkPacket& packet, const std::vector<int> &excludedClients, int channel)
	{
		for (auto& [peer, clientId] : m_ClientIds)
		{
			if (std::find(excludedClients.begin(), excludedClients.end(), clientId) == excludedClients.end())
			{
				ImplSendPacketToClient(clientId, packet, channel);
			}
		}

	}

	void Server::ImplSendPacketToClientWithResponse(int clientId, const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel)
	{
		int callbackId = m_CallbackIdCounter++;
		auto& thisClientCallbacks = m_AwaitingResponseCallbacks[m_ClientIdsReverse.at(clientId)];
		thisClientCallbacks[callbackId] = callback;

		ToServerRequestResponsePacket requestPacket;
		requestPacket.PacketType = Packets::SERVER_REQUEST_RESPONSE;
		requestPacket.RequestId = callbackId;

		ImplSendPacketToClient(clientId, requestPacket, channel);
		ImplSendPacketToClient(clientId, packet, channel);
	}

	void Server::DisconnectClient(int clientId)
	{
		// TODO tell the client they were disconnected
		ENetPeer* peer = m_ClientIdsReverse.at(clientId);
		assert(peer);

		enet_peer_disconnect_now(peer, 0);
	}

	void Server::DisconnectAllClients()
	{
		for (auto& [peer, clientId] : m_ClientIds)
		{
			DisconnectClient(clientId);
		}
	}

	std::vector<int> Server::GetConnectedClientIds() const
	{
		std::vector<int> clientIds;
		clientIds.reserve(m_ClientIds.size());
		for (auto& [peer, clientId] : m_ClientIds)
		{
			clientIds.push_back(clientId);
		}
		return clientIds;
	}

	void Server::AsyncUpdate()
	{
		if (!IsValid()) {
			return;
		}

		// Send queued packets
		if (m_SendQueuedPackets) {
			m_QueuedPacketsToSend.PopAll([this](const std::function<void()>& packet) {
				packet();
				});
			m_SendQueuedPackets = false;
		}

		ENetEvent event;
		while (enet_host_service(m_Server, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				HandleConnectionEvent(event);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				HandleReceiveEvent(event);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				HandleDisconnectionEvent(event);
				break;
			default:
				break;
			}
		}
	}

	void Server::HandleConnectionEvent(const ENetEvent& event)
	{
		assert(event.type == ENET_EVENT_TYPE_CONNECT);

		// Give the client an id
		ToClientSetClientIdPacket idPacket;
		idPacket.PacketType = Packets::CLIENT_SET_ID;
		idPacket.NewClientId = m_NextClientId++;
		m_ClientIds[event.peer] = idPacket.NewClientId;
		m_ClientIdsReverse[idPacket.NewClientId] = event.peer;
		m_AwaitingResponseCallbacks[event.peer] = std::unordered_map<int, std::function<void(const NetworkPacket*)>>();

		ImplSendPacketToClient(idPacket.NewClientId, idPacket);

		// Broadcast connection
		ClientConnectedPacket connectPacket;
		connectPacket.PacketType = Packets::CLIENT_CONNECTED;
		connectPacket.ClientId = idPacket.NewClientId;
		ImplSendPacketToAllClientsExcept(connectPacket, { idPacket.NewClientId });

		std::unique_ptr<ServerClientConnectedPacket> serverConnectPacket = std::make_unique<ServerClientConnectedPacket>();
		serverConnectPacket->PacketType = Packets::SERVER_CLIENT_CONNECTED;
		serverConnectPacket->ClientId = idPacket.NewClientId;
		m_EventQueue.Push(std::make_tuple(serverConnectPacket->PacketType, std::move(serverConnectPacket)));
	}

	void Server::HandleDisconnectionEvent(const ENetEvent& event)
	{
		assert(event.type == ENET_EVENT_TYPE_DISCONNECT);

		// Remove the client id
		int clientId = m_ClientIds[event.peer];
		m_ClientIdsReverse.erase(clientId);
		m_ClientIds.erase(event.peer);
		m_RespondToPacketId.erase(event.peer);
		auto& thisClientCallbacks = m_AwaitingResponseCallbacks[event.peer];
		for (auto& [callbackId, callback] : thisClientCallbacks) {
			callback(nullptr); // Call it with nullptr to indicate the client disconnected
		}
		m_AwaitingResponseCallbacks.erase(event.peer);

		// Broadcast disconnection
		ClientDisconnectedPacket disconnectPacket;
		disconnectPacket.PacketType = Packets::CLIENT_DISCONNECTED;
		disconnectPacket.ClientId = clientId;
		ImplSendPacketToAllClients(disconnectPacket);

		std::unique_ptr<ServerClientDisconnectedPacket> serverDisconnectPacket = std::make_unique<ServerClientDisconnectedPacket>();
		serverDisconnectPacket->PacketType = Packets::SERVER_CLIENT_DISCONNECTED;
		serverDisconnectPacket->ClientId = clientId;
		m_EventQueue.Push(std::make_tuple(serverDisconnectPacket->PacketType, std::move(serverDisconnectPacket)));
	}

	void Server::HandleReceiveEvent(const ENetEvent& event)
	{
		assert(event.type == ENET_EVENT_TYPE_RECEIVE);

		// Parse packet
		std::unique_ptr<NetworkPacket> packet = PacketTypeRegistry::Parse(event.packet->dataLength, (char*)(event.packet->data));
		assert(packet->PacketType != Packets::INVALID);

		// Forward or broadcast packet
		if (packet->PacketType == Packets::FORWARD)
		{
			ToServerForwardPacket* forwardPacket = reinterpret_cast<ToServerForwardPacket*>(packet.get());
			m_ForwardNextPacketToClientId = forwardPacket->ClientId;
		}
		else if (m_ForwardNextPacketToClientId != INVALID_ID)
		{
			ImplSendPacketToClient(m_ForwardNextPacketToClientId, *packet);
			m_ForwardNextPacketToClientId = INVALID_ID;
		}
		// Handle client requesting a response
		else if (packet->PacketType == Packets::CLIENT_REQUEST_RESPONSE)
		{
			ToServerRequestResponsePacket* responsePacket = reinterpret_cast<ToServerRequestResponsePacket*>(packet.get());
			assert(responsePacket);

			// Mark next packet as one that needs to be responded to
			m_RespondToPacketId[event.peer] = responsePacket->RequestId;
		}
		else if (m_RespondToPacketId.find(event.peer) != m_RespondToPacketId.end()) {
			// Get request id
			auto requestId = m_RespondToPacketId[event.peer];

			// Remove request id
			m_RespondToPacketId.erase(event.peer);

			// Get our response
			auto iter = m_PacketResponseHandlers.find(packet->PacketType);
			assert(iter != m_PacketResponseHandlers.end());
			PacketResponseFunc& responseFunc = iter->second;
			std::unique_ptr<NetworkPacket> responsePacket = responseFunc(packet.get());

			// Tell them our next packet is a response
			ToClientServerRespondingPacket response;
			response.PacketType = Packets::SERVER_RESPONDING_TO_CLIENT;
			response.RequestId = requestId;
			ImplSendPacketToClient(m_ClientIds[event.peer], response);

			// Send the response
			ImplSendPacketToClient(m_ClientIds[event.peer], *responsePacket);
		}
		// Handle client responding to us
		else if (packet->PacketType == Packets::CLIENT_RESPONDING_TO_SERVER) {
			// Make the next packet call the callback
			m_IsHandlingCallback = true;
			m_HandlingCallbackId = reinterpret_cast<ToServerClientRespondingPacket*>(packet.get())->RequestId;
		}
		else if (m_IsHandlingCallback) {
			// Handle the callback
			auto& thisClientCallbacks = m_AwaitingResponseCallbacks[event.peer];
			auto iter = thisClientCallbacks.find(m_HandlingCallbackId);
			m_IsHandlingCallback = false;
			if (iter != thisClientCallbacks.end()) {
				iter->second(packet.get());
				thisClientCallbacks.erase(iter);
			}
			else {
				assert(false);
			}
		}

		m_EventQueue.Push(std::make_tuple(packet->PacketType, std::move(packet)));

		// Destroy packet
		enet_packet_destroy(event.packet);
	}
}