#pragma once

#include "stdafx.h"

class ClientCurrentMatchHandler
{
public:
	ClientCurrentMatchHandler(Chimp::Engine& engine, Chimp::IClient& client);
	~ClientCurrentMatchHandler();

	void Update();

	[[nodiscard]] int GetMatchId() const { return m_MatchId; }
	[[nodiscard]] int GetOpponentId() const { return m_OpponentId; }
	[[nodiscard]] bool IsInMatch() const { return m_MatchId != -1; }
	[[nodiscard]] int GetMapFileIndex() const { return m_MapFileIndex; }

private:
	void HandleMatchStart(const Chimp::NetworkPacket* event);

private:
	Chimp::Engine& m_Engine;
	Chimp::IClient& m_Client;
	Chimp::EventListener m_MatchStartListener;
	int m_MatchId = -1;
	int m_OpponentId = -1;
	int m_MapFileIndex = -1;
};