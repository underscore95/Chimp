#include "ServerMatch.h"

ServerMatch::ServerMatch(
	int playerOneId,
	int playerTwoId) :
	m_PlayerOneId(playerOneId),
	m_PlayerTwoId(playerTwoId),
	m_MatchId(s_NextMatchId++),
	m_Result(MatchResult::IN_PROGRESS)
{
}

int ServerMatch::GetPlayerOneId() const
{
	return m_PlayerOneId;
}

int ServerMatch::GetPlayerTwoId() const
{
	return m_PlayerTwoId;
}

int ServerMatch::GetMatchId() const
{
	return m_MatchId;
}

int ServerMatch::s_NextMatchId = 0;