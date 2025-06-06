#include "api/ecs/scripting/IEntityScript.h"
#include "api/IGame.h"
#include "Loggers.h"

namespace Chimp {
	IEntityScript::IEntityScript(std::string_view name, Engine& engine, ECS& ecs) :
		ISystem(engine, ecs),
		m_Name(name),
		m_EntityId(0) {

		// Check if the script is registered
		for (auto& [n, _] : IGame::Instance().GetScripts()) {
			if (name == n) return;
		}
		Loggers::ECS().Warning(std::format("Created script that wasn't registered (name: '{}')", name));
	}
}