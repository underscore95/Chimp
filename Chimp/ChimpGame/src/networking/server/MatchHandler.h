#pragma once

#include "stdafx.h"
#include "MatchSet.h"

class MatchHandler
{
public:
	DISABLE_COPY(MatchHandler);
	MatchHandler(Chimp::Engine& engine, Chimp::IServer& server);
	~MatchHandler();

	void Update();

	void AddMatchEndCallback(std::function<void(const ServerMatch&)> callback);

	std::tuple<int, int> GetPlayersInMatch(int matchId) 
	{
		auto match = m_MatchSet.GetMatchByMatchId(matchId);
		return std::make_tuple(match->GetPlayerOneId(), match->GetPlayerTwoId());
	}

	int MapCount = 1;

private:
	void HandleNewConnections(const Chimp::NetworkPacket* event);
	void HandleMatchEnd(const Chimp::NetworkPacket* event);
	void StartMatch(int player1, int player2);
	void SendMatchEndPacket(int matchId);

private:
	Chimp::Engine& m_Engine;
	Chimp::IServer& m_Server;
	std::queue<int> m_PlayersNotInMatch;
	MatchSet m_MatchSet;
	Chimp::EventListener m_NewConnectionListener;
	Chimp::EventListener m_MatchEndListener;
	std::vector<std::function<void(const ServerMatch&)>> m_MatchEndCallbacks;
};