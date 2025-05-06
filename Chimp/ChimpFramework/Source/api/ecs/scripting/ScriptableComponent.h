#pragma once

#include "stdafx.h"

namespace Chimp {
	class IEntityScript;

	struct ScriptableComponent {
		std::list<std::shared_ptr<IEntityScript>> Scripts;
	};
}