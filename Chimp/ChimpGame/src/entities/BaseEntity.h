#pragma once

#include "stdafx.h"

namespace Entities {
	inline Chimp::EntityId CreateBaseEntity(Chimp::ECS& ecs, Chimp::Mesh& mesh, Chimp::Transform transform = {}) {
		Chimp::EntityId entity = ecs.CreateEntity();

		ecs.SetComponent(entity, Chimp::TransformComponent{ transform });
		ecs.SetComponent(entity, Chimp::MeshComponent{ &mesh });
		ecs.SetComponent(entity, Chimp::EntityIdComponent{ entity });

		return entity;
	}
}