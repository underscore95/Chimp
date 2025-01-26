#pragma once

#include "stdafx.h"

class WaveStartListener {
public:
	WaveStartListener(Chimp::IClient& client);
	~WaveStartListener();

	// Returns true if the wave can start
	bool CanStartNextWave(int waveIndex) const;

private:
	Chimp::EventListener m_Listener;
	int m_StartWaveIndex = -1;
	Chimp::IClient& m_Client;
};