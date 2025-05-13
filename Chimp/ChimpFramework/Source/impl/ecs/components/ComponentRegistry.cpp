#include "api/ecs/components/ComponentRegistry.h"
#include "stdafx.h"

namespace Chimp {
	ComponentRegistry& Chimp::ComponentRegistry::Instance()
	{
		static ComponentRegistry inst;
		return inst;
	}

	void ComponentRegistry::RegisterComponentsInECS(ECS& ecs)
	{
		for (auto& func : m_ECSRegisterFunctions) {
			func(ecs);
		}
	}
}