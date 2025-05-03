#pragma once

#include "stdafx.h"
#include "api/ecs/EntityId.h"

namespace Chimp {
	struct ChildrenComponent {
		std::set<EntityId> Children;
	};
}