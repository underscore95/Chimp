#pragma once

#include "stdafx.h"
#include "api/ecs/components/ComponentRegistry.h"

namespace Chimp {
	// This will be added to entities when selected in hierarchy, we use it so that rotation values don't "randomly" (to the user at least) change since we display euler but use quaternions for logic
	// It will never be removed from entities because then the rotation changes if you click off and go back to it
	// This is required for the editor but should not be used by games
	struct EulerRotationComponent {
		Vector3f Rotation;
	};

	namespace Unused {
		class EulerRotationComponentRegister : public ComponentRegister<EulerRotationComponent> {
		public:
			EulerRotationComponentRegister() : ComponentRegister(true) {}

			EulerRotationComponent Deserialise(const Json& json) override {
				return {
				.Rotation = json["Rotation"]
				};
			}

			void Serialise(Json& json, const EulerRotationComponent& comp) override {
				json["Rotation"] = comp.Rotation;
			}
		};
		COMPONENT_REGISTER(EulerRotationComponentRegister);
	}
}