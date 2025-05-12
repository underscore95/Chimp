#pragma once

#include "stdafx.h"

namespace ChimpEditor {
	struct EntityNameComponent {
		std::string Name;
	};

	namespace Unused {
		class EntityNameComponentRegister : public Chimp::ComponentRegister<EntityNameComponent> {
		public:
			EntityNameComponentRegister() : ComponentRegister(true) {}
		};
		COMPONENT_REGISTER(EntityNameComponentRegister);
	}
}