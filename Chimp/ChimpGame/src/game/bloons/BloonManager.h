#pragma once

#include "game/Entities.h"
#include "Debug.h"

class BloonManager {
public:
	BloonManager(Chimp::Engine& engine,
		Chimp::ECS& ecs,
		Chimp::Vector2f simulationPosition,
		Chimp::YAMLBlock& currentMap);

	void Update();
	void RenderUI();

	void SpawnBloon(Bloons::BloonType type);

	bool HasLost() const;

private:
	void HandleMovement(float dt);

private:
	Chimp::ECS& m_ECS;
	Chimp::Engine& m_Engine;
	Chimp::Path<Chimp::Vector2f> m_Path;
	Chimp::Vector2f m_SimulationPosition;
	static constexpr  int StartingLives =
#ifdef DEBUG_LOTS_OF_LIVES
		100000;
#else
		100;
#endif
	int m_Lives = StartingLives;
	std::array<float, Bloons::NUM_BLOON_TYPES> m_ConstantSpawningTimes;
	Chimp::Timer m_SimTimer;
};