#pragma once

#include "api/ecs/components/ComponentRegistry.h"

namespace Chimp {
	struct EntityNameComponent {
		std::string Name;
	};

	namespace Unused {
		class EntityNameComponentRegister : public ComponentRegister<EntityNameComponent> {
		public:
			void RenderInspectorUI(EntityId id, EntityNameComponent& comp) override {
				auto nameLabel = std::format("Name##{}", (long)id);
				if (comp.Name.capacity() < 32) comp.Name.resize(32);
				ImGui::InputText(nameLabel.c_str(), &comp.Name[0], comp.Name.capacity() + 1);
			}

			EntityNameComponent Deserialise(const Json& json) override {
				return {
					.Name = json["Name"].get<std::string>()
				};
			}

			void Serialise(Json& json, const EntityNameComponent& comp) override {
				json["Name"] = comp.Name;
			}
		};
		COMPONENT_REGISTER(EntityNameComponentRegister);
	}
}
