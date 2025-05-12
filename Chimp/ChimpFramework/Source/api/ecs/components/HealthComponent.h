#pragma once

#include "api/ecs/components/ComponentRegistry.h"

namespace Chimp {
	struct HealthComponent {
		float Health = 1; // If reaches 0, entity dies (this is not built into chimp, make a system that does this yourself)
	};

	namespace Unused {
		class HealthComponentRegister : ComponentRegister<HealthComponent> {
		public:
			void RenderInspectorUI(EntityId id, HealthComponent& comp) override {
				auto healthLabel = std::format("Health##{}", (long)id);
				ImGui::InputFloat(healthLabel.c_str(), &comp.Health, 1, 10);
			}
		};

		static HealthComponentRegister RegisterHealthComponent;
	}
}