#pragma once

#include "stdafx.h"
#include "networking/Networking.h"
#include "game/Simulation.h"


class MatchWinLoseHandler {
public:
	MatchWinLoseHandler(Chimp::Engine& engine, 
		Simulation& playerSimulation,
		std::shared_ptr<Chimp::GameShader> renderer);

	void Update();

private:
	Chimp::Engine& m_Engine;
	Simulation& m_PlayerSimulation;
	std::shared_ptr<Chimp::GameShader> m_GameShader;
};