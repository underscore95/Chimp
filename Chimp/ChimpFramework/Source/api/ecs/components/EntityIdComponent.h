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

			EntityIdComponent Deserialise(const Json& json) override {
				return {
				.Id = json["Id"]
				};
			}

			void Serialise(Json& json, const EntityIdComponent& comp) override {
				json["Id"] = comp.Id;
			}
		};
		COMPONENT_REGISTER(EntityIdComponentRegister);
	}
}