#pragma once

namespace Chimp {
	typedef int NetworkPacketType;
	namespace Packets {
		// Register these in packet type registry
		// First word is who they are being sent to
		// note in some cases (e.g SERVER_CLIENT_CONNECTED) they are being sent from the server to the server
		static constexpr NetworkPacketType INVALID = -1;
		static constexpr NetworkPacketType CLIENT_SET_ID = -2;
		static constexpr NetworkPacketType FORWARD = -3;
		static constexpr NetworkPacketType TEST = -4;
		static constexpr NetworkPacketType CLIENT_CONNECTED = -5;
		static constexpr NetworkPacketType CLIENT_DISCONNECTED = -6;
		static constexpr NetworkPacketType CLIENT_REQUEST_RESPONSE = -7;
		static constexpr NetworkPacketType SERVER_RESPONDING_TO_CLIENT = -8;
		static constexpr NetworkPacketType SERVER_REQUEST_RESPONSE = -9;
		static constexpr NetworkPacketType CLIENT_RESPONDING_TO_SERVER = -10;
		static constexpr NetworkPacketType SERVER_CLIENT_CONNECTED = -11;
		static constexpr NetworkPacketType SERVER_CLIENT_DISCONNECTED = -12;
	}

	struct NetworkPacket {
		NetworkPacketType PacketType = Packets::INVALID;
	};

	// Forward a packet to another client
	// sending this means the next packet will be forwarded
	struct ToServerForwardPacket : public NetworkPacket {
		int ClientId;
	};

	struct ToClientSetClientIdPacket : public NetworkPacket {
		int NewClientId;
	};

	struct TestPacket : public NetworkPacket {
		int TestInt;
	};

	struct ClientConnectedPacket : public NetworkPacket {
		int ClientId;
	};

	struct ClientDisconnectedPacket : public NetworkPacket {
		int ClientId;
	};

	struct ServerClientConnectedPacket : public NetworkPacket {
		int ClientId;
	};

	struct ServerClientDisconnectedPacket : public NetworkPacket {
		int ClientId;
	};

	struct ToServerRequestResponsePacket : public NetworkPacket {
		int RequestId;
	};

	struct ToClientServerRespondingPacket : public NetworkPacket {
		int RequestId;
	};

	struct ToClientRequestResponsePacket : public NetworkPacket {
		int RequestId;
	};

	struct ToServerClientRespondingPacket : public NetworkPacket {
		int RequestId;
	};
}