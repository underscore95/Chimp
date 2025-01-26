#include "WaveStartListener.h"
#include "networking/Networking.h"
#include "Logger.h"

WaveStartListener::WaveStartListener(Chimp::IClient& client)
	: m_Client(client)
{
	m_Listener = m_Client.GetEventHandler().Subscribe(
		Networking::CLIENT_START_WAVE,
		[this](const Chimp::NetworkPacket* packet) {
			const auto wavePacket = static_cast<const ClientStartWavePacket*>(packet);
			assert(wavePacket);
			if (m_StartWaveIndex < (int) wavePacket->WaveIndex)
			{
				m_StartWaveIndex = (int) wavePacket->WaveIndex;
			}
		}
	);
}

WaveStartListener::~WaveStartListener()
{
	m_Client.GetEventHandler().Unsubscribe(m_Listener);
}

bool WaveStartListener::CanStartNextWave(int waveIndex) const
{
	return m_StartWaveIndex >= waveIndex;
}
