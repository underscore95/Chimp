#pragma once

#include "stdafx.h"

class HealthSystem {
public:
	HealthSystem(Chimp::ECS& ecs) : m_ECS(ecs) {}

	void Update() {
		std::vector<Chimp::EntityId> entitiesToRemove;
		auto entities = m_ECS.GetEntitiesWithComponents<Chimp::HealthComponent, Chimp::EntityIdComponent>();
		for (auto& [health, entIdComp] : entities) {
			if (health.Health <= 0) {
				entitiesToRemove.push_back(entIdComp.Id);
			}
		}

#ifndef NDEBUG
		// Make sure all entities with health have an entity id component
		auto entitiesWithHealth = m_ECS.GetEntitiesWithComponents<Chimp::HealthComponent>();
		assert(entitiesWithHealth.Size() == entities.Size());
#endif

		for (auto entity : entitiesToRemove) {
			m_ECS.RemoveEntity(entity);
		}
	}

private:
	Chimp::ECS& m_ECS;
};