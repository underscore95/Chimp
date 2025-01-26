#include "MonkeyTowerComponent.h"
#include "entities/BaseEntity.h"
#include "entities/towers/TowerTypes.h"

Chimp::EntityId Entities::CreateMonkeyTower(Chimp::ECS& ecs, Chimp::Mesh& mesh, Chimp::Vector2f position)
{
	constexpr float scale = 45;
	Chimp::EntityId ent = Entities::CreateBaseEntity(ecs, mesh, Chimp::Transform(
		{ position.x, position.y, TOWER_Z },
		{ 0.0f, 0.8f, 6.0f },
		{ scale, scale, scale }
	));

	ecs.SetComponent(ent, MonkeyTowerComponent{});
	ecs.SetComponent(ent, Chimp::HealthComponent{});
	ecs.SetComponent(ent, TowerComponent{ TOWER_TYPE_DART_MONKEY });
	ecs.SetComponent(ent, WorthComponent{ TOWER_COSTS[TOWER_TYPE_DART_MONKEY] });
	ecs.SetComponent(ent, UpgradableComponent{});
	ecs.SetComponent(ent, NetworkedIdentifierComponent{});

	return ent;
}
