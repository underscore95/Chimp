#pragma once

#include "stdafx.h"
#include "game/Entities.h"

class TowerUpgradeDisplaySystem {
public:
	TowerUpgradeDisplaySystem(Chimp::Engine& engine, Chimp::ECS& ecs);

	void RenderUI();

private:
	Chimp::Engine& m_Engine;
	Chimp::ECS& m_ECS;
};