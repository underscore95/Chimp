#include "TowerProjectileSystem.h"
#include "menu/Settings.h"

TowerProjectileSystem::TowerProjectileSystem(Chimp::Engine& engine,
	Chimp::ECS& ecs,
	Chimp::Vector2f simulationPosition,
	Chimp::Vector2f simulationSize) :
	m_Engine(engine),
	m_ECS(ecs),
	m_SimulationBounds(simulationPosition, simulationSize)
{
}

void TowerProjectileSystem::Update()
{
	const float dt = m_Engine.GetTimeManager().GetDeltaTime();
	auto projectiles = m_ECS.GetEntitiesWithComponents<ProjectileComponent, Chimp::TransformComponent, Chimp::HealthComponent>();

	auto bloons = m_ECS.GetEntitiesWithComponents<BloonComponent, Chimp::TransformComponent, Chimp::EntityIdComponent>();
	std::vector<Chimp::Vector2f> bloonPositions;
	bloonPositions.reserve(bloons.Size());
	for (size_t i = 0; i < bloons.Size(); i++) {
		auto [bloon, transform, health] = bloons.GetByIndex(i);
		bloonPositions.push_back({
			transform.GetTranslation().x + transform.GetScale().x * 0.5f,
			transform.GetTranslation().y + transform.GetScale().y * -0.5f
			});
	}

	std::vector<std::function<void()>> tasks;

	for (auto& [projectile, projTransform, projHealth] : projectiles) {
		// Check for collisions
		tasks.push_back([this, &bloonPositions, dt, &projectile, &projTransform, &projHealth, &bloons]() {
			// Handle lifetimes
			projectile.SecondsRemaining -= dt;
			const bool inBounds = m_SimulationBounds.Contains(Chimp::ComponentMultiply(projTransform.GetTranslation(), { 1, -1, 1 }));
			if (projectile.SecondsRemaining <= 0 || !inBounds) {
				projHealth.Health = 0;
				return;
			}

			// Move the projectile
			const auto& velocity = projectile.Velocity;
			projTransform.Move(velocity.x * dt, velocity.y * dt, 0);
			const auto projPos = projTransform.GetTranslation() + Chimp::ComponentMultiply({ 0.5, -0.5, 0.0 }, projTransform.GetScale());

			// Handle collisions
			const float damageRadiusSquared = Chimp::SquaredLength(projTransform.GetScale());
			for (size_t i = 0; i < bloonPositions.size(); i++) {
				const auto& bloonPos = bloonPositions[i];
				if (Chimp::GetSquaredDistanceBetween({ projPos.x, projPos.y }, bloonPos) < damageRadiusSquared) {
					auto& [bloon, bloonTransform, bloonId] = bloons.GetByIndex(i);
					Bloons::DamageBloon(m_ECS, m_Engine, bloonId.Id, projectile.Damage);
					m_Engine.GetResourceManager().GetSoundEffects().Get(GAME_DATA_FOLDER + std::string("/Assets/Sounds/Bloon.yml")).PlayForce({ bloonPos.x, -bloonPos.y, 0.0f }, {}, Settings::SoundVolume);
					if (--projHealth.Health <= 0) {
						break;
					}
				}
			}
			});
	}

	m_Engine.GetThreadPool().WaitUntilTasksExecuted(tasks);
}
