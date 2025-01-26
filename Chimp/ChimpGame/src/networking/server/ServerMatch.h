#pragma once

#include "networking/matches/MatchResult.h"

class ServerMatch {
public:
	ServerMatch(int playerOneId, int playerTwoId);

	int GetPlayerOneId() const;
	int GetPlayerTwoId() const;
	int GetMatchId() const;

private:
	int m_PlayerOneId;
	int m_PlayerTwoId;
	int m_MatchId;
	MatchResult m_Result;

	static int s_NextMatchId;
};