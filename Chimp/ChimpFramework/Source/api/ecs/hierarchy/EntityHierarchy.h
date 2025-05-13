#pragma once

#include "api/utils/UnorderedCollection.h"
#include "api/ecs/EntityId.h"

namespace Chimp {
	class ECS;
	class EntityHierarchy {
		friend class ECS;
	private:
		EntityHierarchy(ECS& ecs);

	public:
		static constexpr int SMALL_CHILDREN_SIZE = 3;

	public:
		void SetParent(EntityId child, EntityId parent);

		bool IsChildOf(EntityId parent, EntityId possibleChild) const;

		void OrphanChild(EntityId child) const;

		const UnorderedCollection<EntityId, SMALL_CHILDREN_SIZE>& GetChildren(EntityId parent) const;

		EntityId GetParent(EntityId child);

		bool TryGetParent(EntityId child, EntityId& outParent);

	private:
		void OnCreateEntity(EntityId createdEntity);

		void RemoveEntityAndChildren(EntityId entity); 
		void RemoveEntityRecursive(EntityId entity);

	private:
		ECS& m_ECS;
	};
}