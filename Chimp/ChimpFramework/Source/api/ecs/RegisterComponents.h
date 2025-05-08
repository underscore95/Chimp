#pragma once

#include "api/ecs/components/ComponentRegistry.h"
#include "api/ecs/components/HealthComponent.h"
#include "api/ecs/hierarchy/HierarchyComponent.h"
#include "api/ecs/components/MeshComponent.h"
#include "api/ecs/scripting/ScriptableComponent.h"

namespace Chimp::Unused {
	static ComponentRegister<HealthComponent> B;
	static ComponentRegister<MeshComponent> D;
	static ComponentRegister<ScriptableComponent> E;
}