#pragma once

#include "api/networking/IServer.h"
#include "InitENet.h"

namespace Chimp {
	class Server : public IServer {
	public:
		Server(const ConnectionInfo& serverInfo);
		~Server();
	public:
		bool IsValid() const override;

		void ImplSendPacketToClient(int clientId, const NetworkPacket& packet, int channel = 0) override;

		void ImplSendPacketToAllClients(const NetworkPacket& packet, int channel = 0) override;

		void ImplSendPacketToAllClientsExcept(const NetworkPacket& packet, const std::vector<int>& excludedClients, int channel = 0) override;

		void ImplSendPacketToClientWithResponse(int clientId, const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel = 0) override;

		void DisconnectClient(int clientId) override;

		void DisconnectAllClients() override;

		std::vector<int> GetConnectedClientIds() const override;

	protected:
		void AsyncUpdate() override;

	private:
		void HandleConnectionEvent(const ENetEvent& event);
		void HandleDisconnectionEvent(const ENetEvent& event);
		void HandleReceiveEvent(const ENetEvent& event);

	private:
		bool m_IsValid = false;
		ENetHost* m_Server;
		std::unordered_map<ENetPeer*, int> m_ClientIds;
		std::unordered_map<int, ENetPeer*> m_ClientIdsReverse;
		int m_NextClientId = 0;
		int m_ForwardNextPacketToClientId = INVALID_ID;
		std::unordered_map<ENetPeer*, int> m_RespondToPacketId; // client, request id
		int m_CallbackIdCounter = 0;
		std::unordered_map<ENetPeer*, std::unordered_map<int, std::function<void(const NetworkPacket*)>>> m_AwaitingResponseCallbacks;
		int m_HandlingCallbackId = 0;
		bool m_IsHandlingCallback = false;
	};
}