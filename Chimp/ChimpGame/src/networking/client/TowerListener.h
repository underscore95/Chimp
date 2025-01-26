#pragma once

#include "stdafx.h"
#include "networking/packets/Packets.h"
#include "game/towers/TowerManager.h"

class TowerListener {
public:
	TowerListener(Chimp::IClient& client);
	~TowerListener();

	void Update(TowerManager& opponentTowerManager);

private:
	std::queue<ClientTowerPlacePacket> m_PlacePackets;
	Chimp::EventListener m_PlaceListener;
	std::queue<ClientTowerRemovePacket> m_RemovePackets;
	Chimp::EventListener m_RemoveListener;
	std::queue<ClientTowerUpgradePacket> m_UpgradePackets;
	Chimp::EventListener m_UpgradeListener;
	Chimp::IClient& m_Client;
};