#pragma once

#include "stdafx.h"
#include "game/Entities.h"

class TowerSelectionSystem
{
public:
	TowerSelectionSystem(Chimp::Engine& engine, 
		Chimp::ECS& ecs,
		Chimp::Vector2f simulationPosition);

	void Update();
	void RenderUI();

	// Get the selected tower, making sure it is still alive
	bool IsTowerSelected();
	Chimp::EntityId GetSelectedTower() const;
	void DeselectTower() { m_SelectedTower.reset(); }

private:
	Chimp::Engine& m_Engine;
	Chimp::ECS& m_ECS;
	Chimp::Vector2f m_SimulationPosition;
	std::optional<Chimp::EntityId> m_SelectedTower;
};