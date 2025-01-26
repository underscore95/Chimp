#include "TowerManager.h"
#include "Logger.h"
#include "menu/Settings.h"

TowerManager::TowerManager(
	Chimp::Engine& engine,
	Chimp::ECS& ecs,
	Chimp::Vector2f simulationPosition,
	Chimp::Vector2f simulationSize) :
	m_Engine(engine),
	m_ECS(ecs),
	m_SimulationPosition(simulationPosition),
	m_TowerTargetSystem(engine, ecs),
	m_TowerRotationSystem(engine, ecs),
	m_TowerProjectileSystem(engine, ecs, simulationPosition, simulationSize),
	m_TowerUpgradeDisplaySystem(engine, ecs)
{
}

void TowerManager::Update()
{
	m_TowerTargetSystem.Update();
	m_TowerRotationSystem.Update();
	m_TowerProjectileSystem.Update();
}

void TowerManager::RenderUI()
{
	m_TowerUpgradeDisplaySystem.RenderUI();
}

Chimp::EntityId TowerManager::PlaceTower(TowerType type, Chimp::Vector2f position)
{
	position += m_SimulationPosition;

	m_Engine.GetResourceManager().GetSoundEffects().Get(GAME_DATA_FOLDER + std::string("/Assets/Sounds/Place.yml")).Play({ position.x, -position.y, 0.0f }, {}, Settings::SoundVolume);

	switch (type) {
	case TOWER_TYPE_DART_MONKEY:
		return Entities::CreateMonkeyTower(m_ECS, m_Engine.GetResourceManager().GetModels().Get(std::string(GAME_DATA_FOLDER) + "/Assets/Models/monkey/MonkeyOBJ.obj"), position);
	default:
		GetLogger().Error("Unknown tower type when placing tower: " + std::to_string(type));
		assert(false);
		break;
	}
}

Chimp::EntityId TowerManager::PlaceTowerWithNetworkId(TowerType type, Chimp::Vector2f position, NetworkId id)
{
	auto entId = PlaceTower(type, position);
	auto networkId = m_ECS.GetMutableComponent<NetworkedIdentifierComponent>(entId);
	networkId->Id = id;
	return entId;
}

void TowerManager::RemoveTowerWithNetworkId(NetworkId id)
{
	auto view = m_ECS.GetEntitiesWithComponents<NetworkedIdentifierComponent, Chimp::HealthComponent>();
	for (auto& [idComp, health] : view) {
		if (idComp.Id == id) {
			health.Health = 0;
			return;
		}
	}
	GetLogger().Warning("Failed to remove tower with network id: " + std::to_string(id));
}

void TowerManager::UpgradeTowerWithNetworkId(NetworkId id, UpgradeType type)
{
	auto view = m_ECS.GetEntitiesWithComponents<NetworkedIdentifierComponent, UpgradableComponent>();
	for (auto& [idComp, upgrade] : view) {
		if (idComp.Id == id) {
			upgrade.Upgrade(type);
			return;
		}
	}
	GetLogger().Warning("Failed to upgrade tower with network id: " + std::to_string(id));
}
