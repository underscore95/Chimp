#include "BloonManager.h"
#include "Debug.h"

BloonManager::BloonManager(
	Chimp::Engine& engine, 
	Chimp::ECS& ecs,
	Chimp::Vector2f simulationPosition,
	Chimp::YAMLBlock& currentMap) :
	m_ECS(ecs),
	m_Engine(engine),
	m_Path(Chimp::Path<Chimp::Vector2f>::Deserialise(currentMap.Blocks["Path"], simulationPosition)),
	m_SimulationPosition(simulationPosition),
	m_ConstantSpawningTimes({ 0.0f })
{
	m_SimTimer.Start();
}

void BloonManager::Update()
{
	float dt = m_Engine.GetTimeManager().GetDeltaTime();
	HandleMovement(dt);

	Bloons::BloonType type = Bloons::BloonType::RED;
	for (auto& time : m_ConstantSpawningTimes) {
		if (m_SimTimer.GetSecondsElapsed() < 90 + 60 * ((size_t)type)) break;

		time += dt;
		if (time >= 1.0f + 2.0f * (float)(type == Bloons::BloonType::RAINBOW)) {
			SpawnBloon(type);
			time = 0.0f;
		}

		type = (Bloons::BloonType)((size_t)type + 1);
	}
}

void BloonManager::RenderUI()
{
	ImGui::SetCursorPos({ m_SimulationPosition.x, m_SimulationPosition.y });
	std::string livesStr = "Lives: " + std::to_string(m_Lives);
	ImGui::ProgressBar((float)m_Lives / StartingLives, { 200, 20 }, livesStr.c_str());
}

void BloonManager::SpawnBloon(Bloons::BloonType type)
{
	Entities::CreateBloonEntity(
		m_ECS,
		m_Engine,
		m_Path.GetStart(),
		type
	);
}

bool BloonManager::HasLost() const
{
	return m_Lives <= 0;
}

void BloonManager::HandleMovement(float dt)
{
	auto view = m_ECS.GetEntitiesWithComponents<MoveableComponent, Chimp::HealthComponent, Chimp::TransformComponent>();
	dt *= DEBUG_BLOON_SPEED_MULTIPLIER;
	for (auto& [moveable, health, transform] : view) {
		moveable.DistanceTravelled += dt * moveable.Speed;

		// Update position
		auto [point, beforePath, afterPath] = m_Path.GetPointByDistance(moveable.DistanceTravelled);

		transform.SetTranslationXY(point);

		// Reached end of path?
		if (!afterPath) continue;

		m_Lives -= health.Health;
		health.Health = 0;
	}
}
