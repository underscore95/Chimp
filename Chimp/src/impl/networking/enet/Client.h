#pragma once

#include "api/networking/IClient.h"
#include "InitENet.h"

namespace Chimp {
	class Client : public IClient {
	public:
		Client(const ConnectionInfo& serverInfo);
		~Client();
	public:
		bool IsValid() const override;

	protected:
		void ImplSendPacketToServer(const NetworkPacket& packet, int channel = 0) override;
		void ImplSendPacketToServerWithResponse(const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel = 0) override;

		void AsyncUpdate() override;

	private:
		void HandleReceiveEvent(const ENetEvent& event);

	private:
		bool m_DidSuccessfullyConnect = false;
		ENetHost* m_Server;
		ENetPeer* m_Peer;
		std::condition_variable m_ReceiveIdCv;
		int m_CallbackIdCounter = 0;
		std::unordered_map<int, std::function<void(const NetworkPacket*)>> m_AwaitingResponseCallbacks;
		int m_HandlingCallbackId = 0;
		bool m_IsHandlingCallback = false;
		int m_RespondToPacketId = 0;
		bool m_RespondingToPacket = false;
	};
}