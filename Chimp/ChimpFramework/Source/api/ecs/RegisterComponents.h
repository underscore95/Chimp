#pragma once

#include "api/ecs/components/ComponentRegistry.h"
#include "api/ecs/components/EntityIdComponent.h"
#include "api/ecs/components/HealthComponent.h"
#include "api/ecs/components/HierarchyComponent.h"
#include "api/ecs/components/MeshComponent.h"
#include "api/ecs/scripting/ScriptableComponent.h"

namespace Chimp::Unused {
	static ComponentRegister<EntityIdComponent> A;
	static ComponentRegister<HealthComponent> B;
	static ComponentRegister<HierarchyComponent> C;
	static ComponentRegister<MeshComponent> D;
	static ComponentRegister<ScriptableComponent> E;
}