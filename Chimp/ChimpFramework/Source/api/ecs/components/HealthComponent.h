#pragma once

#include "api/ecs/components/ComponentRegistry.h"

namespace Chimp {
	struct HealthComponent {
		float Health = 1; // If reaches 0, entity dies (this is not built into chimp, make a system that does this yourself)
	};

	namespace Unused {
		class HealthComponentRegister : public ComponentRegister<HealthComponent> {
		public:
			void RenderInspectorUI(EntityId id, HealthComponent& comp) override {
				auto healthLabel = std::format("Health##{}", (long)id);
				ImGui::InputFloat(healthLabel.c_str(), &comp.Health, 1, 10);
			}

			HealthComponent Deserialise(const Json& json) override {
				return {
				.Health = json["Health"]
				};
			}

			void Serialise(Json& json, const HealthComponent& comp) override {
				json["Health"] = comp.Health;
			}
		};
		COMPONENT_REGISTER(HealthComponentRegister);
	}
}