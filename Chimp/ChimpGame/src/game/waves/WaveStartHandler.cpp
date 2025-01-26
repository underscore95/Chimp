#include "WaveStartHandler.h"

WaveStartHandler::WaveStartHandler(Chimp::WaveManager& playerWaveManager,
	Chimp::WaveManager& opponentWaveManager,
	Chimp::TaskScheduler& scheduler)
	: m_PlayerWaveManager(playerWaveManager),
	m_OpponentWaveManager(opponentWaveManager)
{
	m_PlayerWaveManager.AddWaveFinishedCallback([this, &scheduler](unsigned int waveIndex) {
		scheduler.RunDelayedTask(5.0f, [this, waveIndex]() {
			ServerWaveEndPacket packet = {};
			packet.PacketType = Networking::SERVER_WAVE_END;
			packet.MatchId = Networking::GetClient()->GetHandlers().CurrentMatchHandler->GetMatchId();
			packet.WaveIndex = waveIndex;
			Networking::GetClient()->GetClient().SendPacketToServer(packet);
			});
		});
}

void WaveStartHandler::Update()
{
	auto& clientHandlers = Networking::GetClient()->GetHandlers();

	// Can we start the next wave?
	if (m_PlayerWaveManager.IsCurrentWaveFinished() &&
		clientHandlers.WaveListener->CanStartNextWave((int)m_PlayerWaveManager.GetWave())) {
#ifndef DEBUG_DISABLE_WAVE_SPAWNING
		m_PlayerWaveManager.AllowStartNextWave();
		m_OpponentWaveManager.AllowStartNextWave();
#endif
	}
}
