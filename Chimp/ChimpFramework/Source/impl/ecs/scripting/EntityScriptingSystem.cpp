#include "api/ecs/scripting/EntityScriptingSystem.h"
#include "api/ecs/ECS.h"
#include "api/ecs/components/EntityIdComponent.h"
#include "api/ecs/scripting/ScriptableComponent.h"

namespace Chimp {
	void EntityScriptingSystem::OnUpdate()
	{
		if (m_processingDisabled) return;
		auto view = GetECS().GetEntitiesWithComponents<ScriptableComponent>();
		for (auto& [scripts] : view) {
			for (auto& script : scripts.Scripts) {
				script->OnUpdate();
			}
		}
	}

	void EntityScriptingSystem::OnRender()
	{
		if (m_processingDisabled) return;
		auto view = GetECS().GetEntitiesWithComponents<ScriptableComponent>();
		for (auto& [scripts] : view) {
			for (auto& script : scripts.Scripts) {
				script->OnRender();
			}
		}
	}
	void EntityScriptingSystem::OnRenderUI()
	{
		if (m_processingDisabled) return;
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
		script->SetEntityId(entity);
		scripts.Scripts.push_back(std::move(script));
		if (!m_processingDisabled) {
			scripts.Scripts.back()->OnInit();
		}
		return (ScriptId)scripts.Scripts.back().get();
	}

	ScriptId EntityScriptingSystem::AttachScript(EntityId entity, std::string_view scriptName)
	{
		auto script = IGame::Instance().CreateScript(std::string(scriptName), GetECS());
		return AttachScript(entity, std::move(script));
	}

	void EntityScriptingSystem::DetachScript(EntityId entity, ScriptId script)
	{
		auto& scripts = GetScriptsOn(entity);
		scripts.Scripts.remove_if([script](const std::shared_ptr<IEntityScript>& s) { return s.get() == script; });
	}

	OptionalReference<IEntityScript> EntityScriptingSystem::GetScript(EntityId entity, ScriptId id)
	{
		auto& scripts = GetScriptsOn(entity).Scripts;

		for (const auto& script : scripts)
		{
			if (script && script.get() == id)
			{
				return *script;
			}
		}

		return {};
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

	void EntityScriptingSystem::OnDeserialise()
	{
		auto view = GetECS().GetEntitiesWithComponents<ScriptableComponent, EntityIdComponent>();
		for (auto& [scripts, id] : view) {
			for (auto& script : scripts.Scripts) {
				script->SetEntityId(id);
			}
		}

		if (m_processingDisabled) return;
		for (auto& [scripts, id] : view) {
			for (auto& script : scripts.Scripts) {
				script->OnInit();
			}
		}
	}
}