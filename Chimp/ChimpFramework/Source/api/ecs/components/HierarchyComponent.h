#pragma once

#include "api/ecs/EntityId.h"
#include "api/utils/UnorderedPossiblyUniqueCollection.h"

namespace Chimp {
	struct HierarchyComponent {
		static constexpr int SMALL_CHILDREN_SIZE = 3;
		EntityId Parent;
		int HierarchyLevel = 0; // 0 = no parent, 1 = 1 parent, etc
		UnorderedPossiblyUniqueCollection<EntityId, SMALL_CHILDREN_SIZE> Children;
	};
}