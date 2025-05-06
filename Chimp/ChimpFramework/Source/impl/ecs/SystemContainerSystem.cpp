#include "api/ecs/SystemContainerSystem.h"
#include "api/ecs/scripting/EntityScriptingSystem.h"
#include "api/Engine.h"

namespace Chimp {
	SystemContainerSystem::SystemContainerSystem(Engine& engine, ECS& ecs) :
		ISystem(engine,ecs)
	{
		
	}

	void SystemContainerSystem::RegisterSystem(std::unique_ptr<ISystem> system)
	{
		m_Systems.push_back(std::move(system));
	}

	void SystemContainerSystem::OnInit()
	{
		for (auto& system : m_Systems) {
			system->OnInit();
		}
	}

	void SystemContainerSystem::OnUpdate()
	{
		for (auto& system : m_Systems) {
			system->OnUpdate();
		}
	}

	void SystemContainerSystem::OnRender()
	{
		for (auto& system : m_Systems) {
			system->OnRender();
		}
	}
	void SystemContainerSystem::OnRenderUI()
	{
		for (auto& system : m_Systems) {
			system->OnRenderUI();
		}
	}
}