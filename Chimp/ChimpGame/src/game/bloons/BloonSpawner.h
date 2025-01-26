#pragma once

#include "stdafx.h"
#include "BloonManager.h"
#include "game/Entities.h"
#include "game/MoneyManager.h"

class BloonSpawner {
public:
	BloonSpawner(Chimp::Engine& engine, 
		BloonManager& opponentBloonManager,
		MoneyManager& moneyManager,
		Chimp::TaskScheduler& taskScheduler,
		Chimp::WaveManager& waveManager);

	void Update();
	void RenderUI();

private:
	void SendBloon(Bloons::BloonType bloonType, float delay = 0);

private:
	Chimp::Engine& m_Engine;
	BloonManager& m_OpponentBloonManager;
	float m_BloonSendsDisabledTime = 0;
	MoneyManager& m_MoneyManager;
	Chimp::TaskScheduler& m_TaskScheduler;
	Chimp::WaveManager& m_WaveManager;
};