#include "BloonsEntities.h"

namespace Entities {
	Chimp::EntityId CreateBloonEntity(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::Vector2f position, Bloons::BloonType type)
	{
		auto& mesh = engine.GetResourceManager().GetSprites().Get(GAME_DATA_FOLDER + std::string("/Assets/Textures/") + Bloons::TexturePaths[(size_t)type]);
		Chimp::EntityId entity = CreateBaseEntity(ecs, mesh, {
			{ position.x, position.y, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 100.0f, 100.0f, 1.0f }
			});

		ecs.SetComponent(entity, Chimp::HealthComponent{ Bloons::HealthValues[(size_t)type]});
		ecs.SetComponent(entity, MoveableComponent{ Bloons::SpeedValues[(size_t)type]});
		ecs.SetComponent(entity, BloonComponent{});

		return entity;
	}
}

void Bloons::DamageBloon(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::EntityId bloonEntity, int damage)
{
	auto health = ecs.GetMutableComponent<Chimp::HealthComponent>(bloonEntity);
	bool createChild = health->Health > 5.0f;
	health->Health -= damage;
	const auto bloonType = HealthToBloonType(health->Health);
	if (createChild) {
		auto bloonChild = Entities::CreateBloonEntity(ecs, engine, { -100, 0 }, bloonType);
		auto bloonChildMoveable = ecs.GetMutableComponent<MoveableComponent>(bloonChild);
		bloonChildMoveable->DistanceTravelled = ecs.GetComponent<MoveableComponent>(bloonEntity)->DistanceTravelled - 20.0f;
	}

	// Update texture
	auto& newMesh = engine.GetResourceManager().GetSprites().Get(GAME_DATA_FOLDER + std::string("/Assets/Textures/") + Bloons::TexturePaths[(size_t)bloonType]);
	ecs.GetMutableComponent<Chimp::MeshComponent>(bloonEntity)->Mesh = &newMesh;
	ecs.GetMutableComponent<MoveableComponent>(bloonEntity)->Speed = Bloons::SpeedValues[(size_t)bloonType];
}
