#pragma once

#include "api/ecs/ISystem.h"
#include "api/ecs/EntityId.h"
#include "api/utils/preprocessor/CopyAndMove.h"

namespace Chimp {
	class IEntityScript : public ISystem {
		DISABLE_COPY(IEntityScript);
	public:
		IEntityScript(EntityId entityId, Engine& engine, ECS& ecs) :
			ISystem(engine, ecs),
			EntityId(entityId) {
		}

		virtual ~IEntityScript() {};

	protected:
		const EntityId EntityId;
	};
}