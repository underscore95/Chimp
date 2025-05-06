#pragma once

#include "ScriptableComponent.h"
#include "api/ecs/SystemContainerSystem.h"
#include "api/ecs/EntityId.h"
#include "api/ecs/scripting/IEntityScript.h"

namespace Chimp {
	typedef void* ScriptId;

	class EntityScriptingSystem : public ISystem {
	public:
		EntityScriptingSystem(Engine& engine, ECS& ecs)
			: ISystem(engine, ecs) {
		}

	public:
		ScriptId AttachScript(EntityId entity, std::unique_ptr<IEntityScript> script);
		void DetachScript(EntityId entity, ScriptId script);

		// returns the first script on the entity which matches the predicate
		OptionalReference<IEntityScript> GetFirstScript(EntityId entity, std::function<bool(const IEntityScript&)> predicate);

		// returns the first script on the entity which is of type T
		template <typename T>
		OptionalReference<T> GetFirstScript(EntityId entity) {
			static_assert(std::is_base_of<IEntityScript, T>::value);
			OptionalReference<IEntityScript> script = GetFirstScript(entity, [](const IEntityScript& script) {
				return typeid(script) == typeid(T); // TODO: This is bad but dynamic_cast wasn't working :(
				});
			return { (T*)(script.GetNullablePtr()) };
		}

	private:
		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnRenderUI() override;

		ScriptableComponent& GetScriptsOn(EntityId entity);
	};
}