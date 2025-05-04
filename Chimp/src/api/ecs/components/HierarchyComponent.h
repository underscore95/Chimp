#pragma once

#include "api/ecs/EntityId.h"
#include "api/utils/UnorderedPossiblyUniqueCollection.h"

namespace Chimp {
	struct HierarchyComponent {
		static constexpr int SMALL_CHILDREN_SIZE = 3;
		UnorderedPossiblyUniqueCollection<EntityId, SMALL_CHILDREN_SIZE> Children;
		EntityId Parent;
		bool HasParent = false;
	};
}