#include "MatchWinLoseHandler.h"
#include "gameover/GameOverScene.h"

MatchWinLoseHandler::MatchWinLoseHandler(Chimp::Engine& engine,
	Simulation& playerSimulation,
	std::shared_ptr<Chimp::GameShader> renderer) :
	m_Engine(engine),
	m_PlayerSimulation(playerSimulation),
	m_GameShader(renderer)
{
}

void MatchWinLoseHandler::Update()
{
	auto& clientHandlers = Networking::GetClient()->GetHandlers();
	
	// Did we lose?
	if (m_PlayerSimulation.HasLost()) {
		// Send win packet to opponent
		ClientMatchWinPacket packet = {};
		packet.PacketType = Networking::CLIENT_MATCH_WIN;
		packet.MatchId = clientHandlers.CurrentMatchHandler->GetMatchId();
		Networking::GetClient()->GetClient().SendPacketToClient(
			clientHandlers.CurrentMatchHandler->GetOpponentId(),
			packet
		);

		// Send end packet to server
		ServerMatchEndPacket endPacket = {};
		endPacket.PacketType = Networking::SERVER_MATCH_END;
		endPacket.MatchId = clientHandlers.CurrentMatchHandler->GetMatchId();
		Networking::GetClient()->GetClient().SendPacketToServer(endPacket);

		m_Engine.GetSceneManager().QueueSceneChange(std::make_unique<GameOverScene>(m_Engine, m_GameShader, false));
	}
	// Did we win?
	else if (clientHandlers.WinListener->DidWinMatch(clientHandlers.CurrentMatchHandler->GetMatchId())) {
		m_Engine.GetSceneManager().QueueSceneChange(std::make_unique<GameOverScene>(m_Engine, m_GameShader, true));
	}
}
