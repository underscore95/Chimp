#pragma once

#include "events/NetworkEventType.h"
#include "stdafx.h"
#include "Loggers.h"

namespace Chimp {
	class Engine;
	class PacketTypeRegistry {
		friend class Engine;
	private:
		PacketTypeRegistry() = default;

		static PacketTypeRegistry& GetInstance() {
			static PacketTypeRegistry instance;
			return instance;
		}
	public:
		// Registers a packet type with the registry
		// type - Packet type id
		template <typename T>
		static void RegisterPacketType(NetworkPacketType type) {
			size_t typeSize = sizeof(T);
			std::function<std::unique_ptr<NetworkPacket>()> factoryFunc = []() { return std::unique_ptr<NetworkPacket>(static_cast<NetworkPacket*>(new T())); };
			auto iter = GetInstance().m_PacketTypeMap.find(type);
			if (iter != GetInstance().m_PacketTypeMap.end()) {
				Loggers::Network().Warning("Registered packet with type " + std::to_string(type) + " already exists, overwriting...");
			}

			GetInstance().m_PacketTypeMap[type] = std::make_tuple(typeSize, factoryFunc);
		}

		// Get the size of a packet type in bytes
		// type - Packet type id
		static size_t GetPacketSize(NetworkPacketType type) {
			auto iter = GetInstance().m_PacketTypeMap.find(type);
			if (iter == GetInstance().m_PacketTypeMap.end()) {
				assert(false);
				return sizeof(NetworkPacket);
			}

			return std::get<0>(iter->second);
		}

		// Convert data into the correct packet type, returns a nullptr if the packet type is not registered
		static std::unique_ptr<NetworkPacket> Parse(size_t size, char* data) {
			NetworkPacketType type = *reinterpret_cast<NetworkPacketType*>(data);
			auto iter = GetInstance().m_PacketTypeMap.find(type);
			if (iter == GetInstance().m_PacketTypeMap.end()) {
				assert(false);
				return nullptr;
			}

			size_t thisPacketSize = std::get<0>(iter->second);
			std::unique_ptr<NetworkPacket> packet = std::get<1>(iter->second)();
			assert(size == thisPacketSize);
			memcpy(packet.get(), data, size);
			return std::move(packet);
		}

	private:
		static void RegisterChimpPacketTypes() {
			RegisterPacketType<NetworkPacket>(Packets::INVALID);
			RegisterPacketType<ToClientSetClientIdPacket>(Packets::CLIENT_SET_ID);
			RegisterPacketType<ToServerForwardPacket>(Packets::FORWARD);
			RegisterPacketType<TestPacket>(Packets::TEST);
			RegisterPacketType<ClientConnectedPacket>(Packets::CLIENT_CONNECTED);
			RegisterPacketType<ClientDisconnectedPacket>(Packets::CLIENT_DISCONNECTED);
			RegisterPacketType<ToServerRequestResponsePacket>(Packets::CLIENT_REQUEST_RESPONSE);
			RegisterPacketType<ToClientServerRespondingPacket>(Packets::SERVER_RESPONDING_TO_CLIENT);
			RegisterPacketType<ToClientRequestResponsePacket>(Packets::SERVER_REQUEST_RESPONSE);
			RegisterPacketType<ToServerClientRespondingPacket>(Packets::CLIENT_RESPONDING_TO_SERVER);
			RegisterPacketType<ServerClientConnectedPacket>(Packets::SERVER_CLIENT_CONNECTED);
			RegisterPacketType<ServerClientDisconnectedPacket>(Packets::SERVER_CLIENT_DISCONNECTED);
		}

	private:
		std::unordered_map<NetworkPacketType, std::tuple<size_t, std::function<std::unique_ptr<NetworkPacket>()>>> m_PacketTypeMap;
	};
}