#pragma once

#include "api/ecs/EntityId.h"
#include "api/utils/UnorderedCollection.h"
#include "EntityHierarchy.h"
#include "api/ecs/components/ComponentRegistry.h"

namespace Chimp {
	struct HierarchyComponent {
		EntityId Parent;
		int HierarchyLevel = 0; // 0 = no parent, 1 = 1 parent, etc
		UnorderedCollection<EntityId, EntityHierarchy::SMALL_CHILDREN_SIZE> Children;
	};

	namespace Unused {
		class HierarchyComponentRegister : public ComponentRegister<HierarchyComponent> {
		public:
			HierarchyComponentRegister() : ComponentRegister(true) {

			}

			HierarchyComponent Deserialise(const Json& json) override {
				return {
				.Parent = SizeTToEntityId(json["Parent"], nullptr),
				.HierarchyLevel = json["HierarchyLevel"],
				.Children = json["Children"]
				};
			}

			void Serialise(Json& json, const HierarchyComponent& comp) override {
				json["Parent"] = comp.Parent.id();
				json["HierarchyLevel"] = comp.HierarchyLevel;
				json["Children"] = comp.Children;
			}
		};
		COMPONENT_REGISTER(HierarchyComponentRegister);
	}
}