#pragma once

#include "stdafx.h"
#include "ConnectionInfo.h"
#include "EventHandler.h"
#include "events/NetworkEventType.h"
#include "api/threading/ThreadSafeQueue.h"
#include "PacketResponseFunc.h"

namespace Chimp {
	// Client and server do share a lot of similarities, but it is easier to have them separate and just duplicate some code.
	// because it makes sense to have the same code with different function names
	// also yes you could have a base class, but it just makes the code a lot harder to reason about
	class IClient {
	protected:
		// Represents a connection to a server
		IClient(const ConnectionInfo& serverInfo);
	public:
		virtual ~IClient();
	public:
		// If this is false, failed to connect to server
		[[nodiscard]] virtual bool IsValid() const = 0;

		// Returns the event handler for this server
		[[nodiscard]] EventHandler<NetworkPacketType, NetworkPacket>& GetEventHandler() {
			return *m_EventHandlerAndBroadcaster.Handler;
		}

		// Broadcast all polled events
		void Update();

		// Handle responding to a packet
		void SetPacketResponseHandler(NetworkPacketType type, PacketResponseFunc func) {
			m_PacketResponseHandlers[type] = func;
		}

		// Send a packet to the server
		// packet - The packet to send
		// channel - The channel to send the packet on, defaults to 0, make sure this is a valid channel
		// callback - The callback to call when a response is received
		void SendPacketToServer(const NetworkPacket& packet, int channel = 0);
		void SendPacketToServerWithResponse(const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel = 0);

		// Send a packet to another client
		// clientId - The client to send the packet to, make sure this is a valid client
		// packet - The packet to send
		// channel - The channel to send the packet on, defaults to 0, make sure this is a valid channel
		void SendPacketToClient(unsigned int clientId, const NetworkPacket& packet, int channel = 0);

		// Returns true if the client was disconnected from the server abruptly
		// e.g server is shutdown
		[[nodiscard]] bool WasDisconnected() const;

		// Returns the connection id of the client
		[[nodiscard]] unsigned int GetId() const {
			return m_ConnectionId;
		}

	protected:
		virtual void ImplSendPacketToServer(const NetworkPacket& packet, int channel = 0) = 0;
		virtual void ImplSendPacketToServerWithResponse(const NetworkPacket& packet, std::function<void(const NetworkPacket*)> callback, int channel = 0) = 0;
		void ImplSendPacketToClient(unsigned int clientId, const NetworkPacket& packet, int channel = 0);

		// Push events into a queue, this is called as fast as possible in its own thread, also send queued packets
		// it is up to the impl to handle if the server is invalid or if the function is called too early
		virtual void AsyncUpdate() = 0;

		// Shut down the update thread, must be called before destroying the client
		// probably should be called at the beginning of the child destructor
		void ShutdownThreads();

	public:
		constexpr static int HOST_ID = -1;
		constexpr static unsigned int INVALID_ID = -2;

	protected:
		const ConnectionInfo m_ServerInfo;
		ThreadSafeQueue<std::tuple<NetworkPacketType, std::shared_ptr<NetworkPacket>>> m_EventQueue;
		unsigned int m_ConnectionId = INVALID_ID;
		std::unordered_map<NetworkPacketType, PacketResponseFunc> m_PacketResponseHandlers;
		ThreadSafeQueue<std::function<void()>> m_QueuedPacketsToSend;
		bool m_SendQueuedPackets = false;
		bool m_FailedToConnect = false;
		bool m_WasDisconnected = false;

	private:
		std::thread m_EventPollingThread;
		bool m_IsBeingDestroyed = false;

	protected:
		EventHandlerAndBroadcaster<NetworkPacketType, NetworkPacket> m_EventHandlerAndBroadcaster;
	};
}