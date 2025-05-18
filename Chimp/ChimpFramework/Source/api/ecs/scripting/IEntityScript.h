#pragma once

#include "api/ecs/ISystem.h"
#include "api/ecs/EntityId.h"
#include "api/utils/preprocessor/CopyAndMove.h"

namespace Chimp {
	class EntityScriptingSystem;
	class IEntityScript : public ISystem {
		DISABLE_COPY(IEntityScript);
		friend class EntityScriptingSystem;
	public:
		IEntityScript(std::string_view name, Engine& engine, ECS& ecs) :
			ISystem(engine, ecs),
			m_EntityId(0) {
		}

		virtual ~IEntityScript() {};

		const std::string& GetName() const { return m_Name; }

	protected:
		EntityId GetEntity() const { return m_EntityId; }

	private:
		void SetEntityId(EntityId entityId) { m_EntityId = entityId; }

	private:
		EntityId m_EntityId;
		const std::string m_Name;
	};
}