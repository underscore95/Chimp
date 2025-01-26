#pragma once

#include "stdafx.h"
#include "MatchHandler.h"

class WaveHandler
{
public:
	DISABLE_COPY(WaveHandler);
	WaveHandler(Chimp::Engine& engine, Chimp::IServer& server, MatchHandler& matchHandler);
	~WaveHandler();

private:
	void HandleReceivedWaveEndPacket(const Chimp::NetworkPacket* event);
	void HandleMatchEnd(const ServerMatch& match);
	void StartWave(int matchId, int nextWaveIndex);

private:
	std::unordered_map<int, int> m_ReceivedWaveEndPackets; // match id -> wave index
	Chimp::EventListener m_WaveEndListener;
	Chimp::Engine& m_Engine;
	Chimp::IServer& m_Server;
	MatchHandler& m_MatchHandler;
};