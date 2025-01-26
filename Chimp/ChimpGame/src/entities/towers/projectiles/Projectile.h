#pragma once

#include "stdafx.h"

struct ProjectileComponent {
	Chimp::Vector2f Velocity;
	int Damage;
	float SecondsRemaining = 1.0f;
};

namespace Entities {
	inline Chimp::EntityId CreateProjectile(Chimp::ECS& ecs,
		Chimp::Mesh& mesh,
		Chimp::Vector2f position,
		Chimp::Vector2f velocity,
		int damage = 1,
		float rotation = 0) {
		Chimp::EntityId entity = CreateBaseEntity(ecs, mesh, {
			{position.x, position.y, 0.0f},
			{0.0f, 0.0f, rotation},
			{25, 25, 1}
			});

		ecs.SetComponent(entity, ProjectileComponent{ velocity, damage, 5 });
		ecs.SetComponent(entity, Chimp::HealthComponent{ 1 });

		return entity;
	}
}