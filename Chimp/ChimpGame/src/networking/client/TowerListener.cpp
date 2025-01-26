#include "TowerListener.h"
#include "networking/Networking.h"

TowerListener::TowerListener(Chimp::IClient& client)
	: m_Client(client)
{
	m_PlaceListener = m_Client.GetEventHandler().Subscribe(
		Networking::CLIENT_TOWER_PLACE,
		[this](const Chimp::NetworkPacket* packet) {
			const auto placePacket = static_cast<const ClientTowerPlacePacket*>(packet);
			assert(placePacket);
			m_PlacePackets.push(*placePacket);
		}
	);

	m_RemoveListener = m_Client.GetEventHandler().Subscribe(
		Networking::CLIENT_TOWER_REMOVE,
		[this](const Chimp::NetworkPacket* packet) {
			const auto removePacket = static_cast<const ClientTowerRemovePacket*>(packet);
			assert(removePacket);
			m_RemovePackets.push(*removePacket);
		}
	);

	m_UpgradeListener = m_Client.GetEventHandler().Subscribe(
		Networking::CLIENT_TOWER_UPGRADE,
		[this](const Chimp::NetworkPacket* packet) {
			const auto upgradePacket = static_cast<const ClientTowerUpgradePacket*>(packet);
			assert(upgradePacket);
			m_UpgradePackets.push(*upgradePacket);
		}
	);
}

TowerListener::~TowerListener()
{
	m_Client.GetEventHandler().Unsubscribe(m_PlaceListener);
	m_Client.GetEventHandler().Unsubscribe(m_RemoveListener);
	m_Client.GetEventHandler().Unsubscribe(m_UpgradeListener);
}

void TowerListener::Update(TowerManager& opponentTowerManager)
{
	while (!m_PlacePackets.empty()) {
		const auto& packet = m_PlacePackets.front();
		opponentTowerManager.PlaceTowerWithNetworkId(packet.Type, packet.Position, packet.TowerId);
		m_PlacePackets.pop();
	}

	while (!m_RemovePackets.empty()) {
		const auto& packet = m_RemovePackets.front();
		opponentTowerManager.RemoveTowerWithNetworkId(packet.TowerId);
		m_RemovePackets.pop();
	}

	while (!m_UpgradePackets.empty()) {
		const auto& packet = m_UpgradePackets.front();
		opponentTowerManager.UpgradeTowerWithNetworkId(packet.TowerId, packet.UpgradeType);
		m_UpgradePackets.pop();
	}
}
