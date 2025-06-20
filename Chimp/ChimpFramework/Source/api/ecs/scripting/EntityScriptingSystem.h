#pragma once

#include "api/ecs/SystemContainerSystem.h"
#include "api/ecs/EntityId.h"
#include "api/ecs/scripting/IEntityScript.h"

namespace Chimp {
	typedef void* ScriptId;
	class ScriptableComponent;
	class ECS;

	// T is script class
	template <typename T>
	struct ScriptAndEntity {
		Reference<T> Script;
		EntityId Entity;
		ScriptAndEntity(Reference<T> script, EntityId entity) : Script(script), Entity(entity) {}
	};

	class EntityScriptingSystem : public ISystem {
		friend class ECS;
	public:
		EntityScriptingSystem(Engine& engine, ECS& ecs)
			: ISystem(engine, ecs) {
		}

	public:
		// Main use case is for the editor, since we don't want game scripts running in editor mode
		// Re enabling it is probably a bad idea since script inits won't be called, you could make a vector of lambdas to call if you want to add enable processing function
		void DisableProcessing() { m_processingDisabled = true; } 

		ScriptId AttachScript(EntityId entity, std::unique_ptr<IEntityScript> script);
		ScriptId AttachScript(EntityId entity, std::string_view scriptName);
		void DetachScript(EntityId entity, ScriptId script);
		OptionalReference<IEntityScript> GetScript(EntityId entity, ScriptId id);

		// returns the first script on the entity which matches the predicate
		OptionalReference<IEntityScript> GetFirstScript(EntityId entity, std::function<bool(IEntityScript*)> predicate);

		// returns the first script on the entity which is of type T
		template <typename T>
		OptionalReference<T> GetFirstScript(EntityId entity) {
			static_assert(std::is_base_of<IEntityScript, T>::value);
			OptionalReference<IEntityScript> script = GetFirstScript(entity, [](IEntityScript* script) {
				return dynamic_cast<T*>(script) != nullptr;
				});
			return { static_cast<T*>(script.GetNullablePtr()) };
		}

		// returns the first entity with the script, also returns the script
		template <typename T>
		std::unique_ptr<ScriptAndEntity<T>> GetFirstEntityWithScript() {
			std::unique_ptr<ScriptAndEntity<T>> found = nullptr;
			ForEachScriptedEntity([&found, this](EntityId entity, ScriptableComponent& scripts) {
				if (found) return;
				OptionalReference<T> script = GetFirstScript<T>(entity);
				if (script) found = std::make_unique<ScriptAndEntity<T>>(script, entity);
				});
			return found;
		}

	private:
		void OnUpdate() override;
		void OnRender() override;
		void OnRenderUI() override;

		void OnDeserialise();

		ScriptableComponent& GetScriptsOn(EntityId entity);
		void ForEachScriptedEntity(const std::function<void(EntityId entity, ScriptableComponent&)>& func);

	private:
		bool m_processingDisabled = false;
	};
}