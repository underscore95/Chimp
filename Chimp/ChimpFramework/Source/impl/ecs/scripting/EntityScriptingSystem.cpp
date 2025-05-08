#include "api/ecs/scripting/EntityScriptingSystem.h"
#include "api/ecs/ECS.h"
#include "api/ecs/components/EntityIdComponent.h"

namespace Chimp {
	void EntityScriptingSystem::OnInit()
	{
		auto view = GetECS().GetEntitiesWithComponents<ScriptableComponent>();
		for (auto& [scripts] : view) {
			for (auto& script : scripts.Scripts) {
				script->OnInit();
			}
		}
	}

	void EntityScriptingSystem::OnUpdate()
	{
		auto view = GetECS().GetEntitiesWithComponents<ScriptableComponent>();
		for (auto& [scripts] : view) {
			for (auto& script : scripts.Scripts) {
				script->OnUpdate();
			}
		}
	}

	void EntityScriptingSystem::OnRender()
	{
		auto view = GetECS().GetEntitiesWithComponents<ScriptableComponent>();
		for (auto& [scripts] : view) {
			for (auto& script : scripts.Scripts) {
				script->OnRender();
			}
		}
	}
	void EntityScriptingSystem::OnRenderUI()
	{
		auto view = GetECS().GetEntitiesWithComponents<ScriptableComponent>();
		for (auto& [scripts] : view) {
			for (auto& script : scripts.Scripts) {
				script->OnRenderUI();
			}
		}
	}

	ScriptableComponent& EntityScriptingSystem::GetScriptsOn(EntityId entity)
	{
		auto scripts = GetECS().GetMutableComponent<ScriptableComponent>(entity);
		if (!scripts) {
			GetECS().SetComponent<ScriptableComponent>(entity, {});
			scripts = GetECS().GetMutableComponent<ScriptableComponent>(entity);
		}
		return *scripts;
	}

	void EntityScriptingSystem::ForEachScriptedEntity(const std::function<void(EntityId entity, ScriptableComponent&)>& func)
	{
		auto view = GetECS().GetEntitiesWithComponents<EntityIdComponent, ScriptableComponent>();
		for (auto& [id, scripts] : view) {
			func(id.Id, scripts);
		}
	}

	ScriptId EntityScriptingSystem::AttachScript(EntityId entity, std::unique_ptr<IEntityScript> script)
	{
		auto& scripts = GetScriptsOn(entity);
		scripts.Scripts.push_back(std::move(script));
		return (ScriptId)scripts.Scripts.back().get();
	}

	void EntityScriptingSystem::DetachScript(EntityId entity, ScriptId script)
	{
		auto& scripts = GetScriptsOn(entity);
		scripts.Scripts.remove_if([script](const std::shared_ptr<IEntityScript>& s) { return s.get() == script; });
	}

	OptionalReference<IEntityScript> EntityScriptingSystem::GetFirstScript(EntityId entity, std::function<bool(IEntityScript*)> predicate)
	{
		auto scripts = GetECS().GetComponent<ScriptableComponent>(entity);
		if (scripts) {
			for (auto& script : scripts->Scripts) {
				if (predicate(script.get())) {
					return script.get();
				}
			}
		}
		return nullptr;
	}
}