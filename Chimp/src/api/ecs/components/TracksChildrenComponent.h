#pragma once

#include "stdafx.h"
#include "api/ecs/EntityId.h"

namespace Chimp {
	struct TracksChildrenComponent {
		std::set<EntityId> Children;
	};
}