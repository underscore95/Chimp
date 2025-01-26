#include "MatchSet.h"

void MatchSet::AddMatch(const ServerMatch& match)
{
	assert(m_PlayerOneIdMatchId.find(match.GetPlayerOneId()) == m_PlayerOneIdMatchId.end());
	m_PlayerOneIdMatchId[match.GetPlayerOneId()] = match.GetMatchId();

	assert(m_PlayerTwoIdMatchId.find(match.GetPlayerTwoId()) == m_PlayerTwoIdMatchId.end());
	m_PlayerTwoIdMatchId[match.GetPlayerTwoId()] = match.GetMatchId();

	assert(m_Matches.find(match.GetMatchId()) == m_Matches.end());
	m_Matches.insert(std::make_pair(match.GetMatchId(), match));
}

void MatchSet::RemoveMatch(const ServerMatch& match)
{
	RemoveMatchById(match.GetMatchId());
}

ServerMatch MatchSet::RemoveMatchById(int matchId)
{
	auto iter = m_Matches.find(matchId);
	if (iter != m_Matches.end())
	{
		ServerMatch match = iter->second;
		m_PlayerOneIdMatchId.erase(iter->second.GetPlayerOneId());
		m_PlayerTwoIdMatchId.erase(iter->second.GetPlayerTwoId());
		m_Matches.erase(iter);
		return match;
	}
	return {-1, -2};
}

Chimp::OptionalReference<ServerMatch> MatchSet::GetMatchByPlayerId(int playerId)
{
	auto iter = m_PlayerOneIdMatchId.find(playerId);
	if (iter == m_PlayerOneIdMatchId.end())
	{
		iter = m_PlayerTwoIdMatchId.find(playerId);
	}
	return iter == m_PlayerTwoIdMatchId.end() ? 
		Chimp::OptionalReference<ServerMatch>(nullptr) :
		GetMatchByMatchId(iter->second);
}

Chimp::OptionalReference<ServerMatch> MatchSet::GetMatchByMatchId(int matchId)
{
	auto iter = m_Matches.find(matchId);
	return Chimp::OptionalReference<ServerMatch>(iter != m_Matches.end() ? &iter->second : nullptr);
}
