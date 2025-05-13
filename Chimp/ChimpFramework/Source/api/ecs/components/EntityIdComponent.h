#pragma once

#include "api/ecs/EntityId.h"
#include "api/ecs/components/ComponentRegistry.h"

namespace Chimp {
	struct EntityIdComponent {
		EntityId Id;
	};

	namespace Unused {
		class EntityIdComponentRegister : public ComponentRegister<EntityIdComponent> {
		public:
			EntityIdComponentRegister() : ComponentRegister(true) {}
		};
		COMPONENT_REGISTER(EntityIdComponentRegister);
	}
}