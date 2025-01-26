#pragma once

#include "stdafx.h"

class GameWinListener {
public:
	GameWinListener(Chimp::IClient& client);
	~GameWinListener();

	bool DidWinMatch(int matchId) const;

private:
	Chimp::EventListener m_Listener;
	std::unordered_set<int> m_MatchIdsWon;
	Chimp::IClient& m_Client;
};