#pragma once

#include "api/ecs/components/ComponentRegistry.h"
#include "stdafx.h"

namespace Chimp {
	class IEntityScript;

	struct ScriptableComponent {
		std::list<std::shared_ptr<IEntityScript>> Scripts;
	};

	namespace Unused {
		static ComponentRegister<ScriptableComponent> ScriptableComponentRegister;
	}
}