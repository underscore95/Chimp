#pragma once

#include "api/ecs/EntityId.h"
#include "api/ecs/components/ComponentRegistry.h"

namespace Chimp {
	struct EntityIdComponent {
		EntityId Id;
	};

	namespace Unused {
		static ComponentRegister<EntityIdComponent> RegisterEntityIdComponent;
	}
}