#pragma once

#include "GameClient.h"
#include "GameServer.h"
#include "packets/Packets.h"

class Networking {
private:
	Networking() = delete;
public:
	static constexpr Chimp::NetworkPacketType CLIENT_MATCH_START = 0;
	static constexpr Chimp::NetworkPacketType CLIENT_MATCH_WIN = 1;
	static constexpr Chimp::NetworkPacketType SERVER_MATCH_END = 2;
	static constexpr Chimp::NetworkPacketType SERVER_WAVE_END = 3;
	static constexpr Chimp::NetworkPacketType CLIENT_START_WAVE = 4;
	static constexpr Chimp::NetworkPacketType CLIENT_TOWER_PLACE = 5;
	static constexpr Chimp::NetworkPacketType CLIENT_TOWER_REMOVE = 6;
	static constexpr Chimp::NetworkPacketType CLIENT_TOWER_UPGRADE = 7;
	static constexpr Chimp::NetworkPacketType CLIENT_BLOON_SEND = 8;

	static void Init(Chimp::Engine& engine) {
		assert(!client && !server);

		Chimp::PacketTypeRegistry::RegisterPacketType<ClientMatchStartPacket>(CLIENT_MATCH_START);
		Chimp::PacketTypeRegistry::RegisterPacketType<ClientMatchWinPacket>(CLIENT_MATCH_WIN);
		Chimp::PacketTypeRegistry::RegisterPacketType<ServerMatchEndPacket>(SERVER_MATCH_END);
		Chimp::PacketTypeRegistry::RegisterPacketType<ServerWaveEndPacket>(SERVER_WAVE_END);
		Chimp::PacketTypeRegistry::RegisterPacketType<ClientStartWavePacket>(CLIENT_START_WAVE);
		Chimp::PacketTypeRegistry::RegisterPacketType<ClientTowerPlacePacket>(CLIENT_TOWER_PLACE);
		Chimp::PacketTypeRegistry::RegisterPacketType<ClientTowerRemovePacket>(CLIENT_TOWER_REMOVE);
		Chimp::PacketTypeRegistry::RegisterPacketType<ClientTowerUpgradePacket>(CLIENT_TOWER_UPGRADE);
		Chimp::PacketTypeRegistry::RegisterPacketType<ClientSpawnBloonPacket>(CLIENT_BLOON_SEND);

		client = std::make_shared<GameClient>(engine);
		server = std::make_shared<GameServer>(engine);
	}

	static std::shared_ptr<GameClient> GetClient() {
		return client;
	}

	static std::shared_ptr<GameServer> GetServer() {
		return server;
	}

private:
	inline static std::shared_ptr<GameClient> client = nullptr;
	inline static std::shared_ptr<GameServer> server = nullptr;
};