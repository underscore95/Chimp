#pragma once

#include "stdafx.h"
#include "networking/packets/Packets.h"
#include "game/bloons/BloonManager.h"

class BloonListener {
public:
	BloonListener(Chimp::IClient& client);
	~BloonListener();

	void Update(BloonManager& bloonManager);

private:
	Chimp::IClient& m_Client;
	std::queue<ClientSpawnBloonPacket> m_SpawnPackets;
	Chimp::EventListener m_SpawnListener;
};