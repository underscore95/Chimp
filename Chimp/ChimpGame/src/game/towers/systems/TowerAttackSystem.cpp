#include "TowerAttackSystem.h"
#include "Debug.h"

TowerAttackSystem::TowerAttackSystem(Chimp::Engine& engine,
	Chimp::ECS& ecs) :
	m_Engine(engine),
	m_ECS(ecs)
{
}

void TowerAttackSystem::Update()
{
	auto towersView = m_ECS.GetEntitiesWithComponents<TowerComponent, UpgradableComponent, Chimp::TransformComponent>();

	for (auto& [tower, upgrades, transform] : towersView) {
		if (tower.Target.Empty()) {
			continue;
		}

		// Look at nearest bloon
		const auto& bloonTransform = m_ECS.GetComponent<Chimp::TransformComponent>(tower.Target.Value());
		const auto bloonPos = bloonTransform->GetTranslation()
			+ Chimp::ComponentMultiply({ 0.5, -0.5, 0.0 }, bloonTransform->GetScale());
		const auto& towerPos = transform.GetTranslation()
			+ Chimp::ComponentMultiply({ 0.5, -0.5, 0.0 }, transform.GetScale());
		Chimp::Vector2f direction = { bloonPos.x - towerPos.x, bloonPos.y - towerPos.y };
		const auto angle = Chimp::Atan2(direction);
		transform.SetRoll(angle - Chimp::PI / 2);

		// Fire the tower
		tower.SecondsUntilNextAttack -= m_Engine.GetTimeManager().GetDeltaTime() * DEBUG_TOWER_ATTACK_SPEED_MULTIPLIER * upgrades.GetAttackSpeedMultiplier();
		if (tower.SecondsUntilNextAttack <= 0) {
			tower.SecondsUntilNextAttack = tower.AttackInterval;
			Entities::CreateProjectile(
				m_ECS,
				m_Engine.GetResourceManager().GetSprites().Get(GAME_DATA_FOLDER + std::string("/Assets/Textures/Dart.png")),
				Chimp::Vector2f(transform.GetTranslation()),
				Chimp::VectorNormalized(direction) * DEBUG_PROJECTILE_SPEED_MULTIPLIER,
				upgrades.GetDamage(tower.AttackDamage),
				angle - Chimp::PI / 2
			);
		}
	}
}
