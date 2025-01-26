#include "GameWinListener.h"
#include "networking/Networking.h"
#include "Logger.h"

GameWinListener::GameWinListener(Chimp::IClient& client) :
	m_Client(client)
{
	m_Listener = m_Client.GetEventHandler().Subscribe(
		Networking::CLIENT_MATCH_WIN,
		[this](const Chimp::NetworkPacket* packet) {
			const auto winPacket = static_cast<const ClientMatchWinPacket*>(packet);
			assert(winPacket);
			m_MatchIdsWon.insert(winPacket->MatchId);
		}
	);
}

GameWinListener::~GameWinListener()
{
	m_Client.GetEventHandler().Unsubscribe(m_Listener);
}

bool GameWinListener::DidWinMatch(int matchId) const
{
	return std::find(m_MatchIdsWon.begin(), m_MatchIdsWon.end(), matchId) != m_MatchIdsWon.end();
}
