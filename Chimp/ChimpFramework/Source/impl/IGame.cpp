#include "api/IGame.h"
#include "Loggers.h"
#include "api/ecs/scripting/IEntityScript.h"
#include "api/ecs/scripting/ScriptableComponent.h"
#include "api/ecs/ECS.h"
#include "api/Engine.h"

namespace Chimp {
	IGame* IGame::g_Instance = nullptr;

	IGame::IGame()
	{
		g_Instance = this;
	}

	IGame& IGame::Instance()
	{
		assert(g_Instance != nullptr);
		return *g_Instance;
	}

	void IGame::RegisterScript(const std::string& name, const CreateScriptFunc& func)
	{
		if (name.size() <= ScriptableComponent::MAX_SCRIPT_NAME_LENGTH) {
			m_Scripts[name] = func;
		}
		else {
			Loggers::ECS().Error(std::format("Script name '{}' exceeds maximum length {}, failed to register. (is {} characters)",
				name, ScriptableComponent::MAX_SCRIPT_NAME_LENGTH, name.size()));
		}
	}

	const std::unordered_map<std::string, IGame::CreateScriptFunc>& IGame::GetScripts() const
	{
		return m_Scripts;
	}

	std::unique_ptr<IEntityScript> IGame::CreateScript(const std::string& name, Engine& engine, ECS& ecs)
	{
		auto it = m_Scripts.find(name);
		if (it != m_Scripts.end()) {
			return (it->second)(engine, ecs);
		}

		Loggers::ECS().Error(std::format("Failed to find script {}, have you registered it?", name));
		assert(false);
		return nullptr;
	}
}