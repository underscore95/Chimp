#pragma once

#include "stdafx.h"
#include "game/Entities.h"

class TowerProjectileSystem
{
public:
	TowerProjectileSystem(Chimp::Engine& engine,
		Chimp::ECS& ecs,
		Chimp::Vector2f simulationPosition,
		Chimp::Vector2f simulationSize);

	void Update();

private:
	Chimp::Engine& m_Engine;
	Chimp::ECS& m_ECS;
	Chimp::Rect m_SimulationBounds;
};