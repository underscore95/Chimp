#include "api/ecs/components/ComponentRegistry.h"
#include "stdafx.h"

namespace Chimp {
	ComponentRegistry& Chimp::ComponentRegistry::Instance()
	{
		static ComponentRegistry inst;
		return inst;
	}
}