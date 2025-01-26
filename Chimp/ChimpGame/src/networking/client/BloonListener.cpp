#include "BloonListener.h"
#include "networking/Networking.h"

BloonListener::BloonListener(Chimp::IClient& client) :
	m_Client(client)
{
	m_SpawnListener = m_Client.GetEventHandler().Subscribe(
			Networking::CLIENT_BLOON_SEND,
			[this](const Chimp::NetworkPacket* packet) {
				const auto spawnPacket = static_cast<const ClientSpawnBloonPacket*>(packet);
				assert(spawnPacket);
				m_SpawnPackets.push(*spawnPacket);
			}
	);
}

BloonListener::~BloonListener()
{
	m_Client.GetEventHandler().Unsubscribe(m_SpawnListener);
}

void BloonListener::Update(BloonManager& bloonManager)
{
	while (!m_SpawnPackets.empty()) {
		const auto& packet = m_SpawnPackets.front();
		bloonManager.SpawnBloon(packet.BloonType);
		m_SpawnPackets.pop();
	}
}
