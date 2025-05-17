#pragma once

#include "stdafx.h"
#include "api/ecs/ECS.h"

namespace Chimp {

	// typedef ComponentRegister scary

	template <typename T>
	class ComponentRegister;
	class Engine;

	class ComponentRegistry {
		template <typename T>
		friend class ComponentRegister;
		friend class Engine;
		DISABLE_COPY_AND_MOVE(ComponentRegistry);
	private:
		ComponentRegistry() = default;
	public:
		static ComponentRegistry& Instance();

		// Renders the editor inspector UI for this type
		void RenderEditorUI(EntityId id, AnyReference value);

		// Serialises component and stores in json[typename]
		void Serialise(Json& json, AnyReference component);

		// Deserialises the component stored in json[typeName] and add it to entity
		void Deserialise(ECS&, EntityId entity, const std::string& typeName, const Json& json);

		bool ShouldHideInInspectorUI(TypeInfo typeInfo) {
			return m_ShouldHideInInspectorUI.contains(typeInfo.Hash());
		}

		size_t GetSize(AnyReference value) {
			size_t hashCode = value.GetType().Hash();
			auto it = m_ComponentSizes.find(hashCode);
			assert(it != m_ComponentSizes.end());
			return it->second;
		}

	public:
		void SetActiveECS(ECS& ecs) {
			for (auto& [hashCode, function] : m_SetActiveECSFunctions) {
				function(ecs);
			}
		}

		// Register a component, this can only be called from ComponentRegister which ensures it is impossible to register a component twice
		template <typename T>
		void RegisterComponent(bool shouldHideInInspectorUI,
			const std::function<void(EntityId, T&)>& renderInspectorUi,
			const std::function<void(ECS&)>& setActiveEcsFunction,
			const std::function<void(Json&, const T&)>& serialiseFunction,
			const std::function<T(const Json&)>& deserialiseFunction
		) {
			TypeInfo typeId = typeid(T);
			size_t hashCode = typeId.Hash();
			assert(!m_RenderEditorUIFunctions.contains(hashCode)); // todo use std::type_index instead of hash code
			m_RenderEditorUIFunctions[hashCode] = [renderInspectorUi](EntityId id, void* value) { renderInspectorUi(id, *reinterpret_cast<T*>(value)); }; // convert the function into one that takes a void*
			m_ECSRegisterFunctions.push_back([](ECS& ecs) { ecs.RegisterComponent<T>(); });
			m_SetActiveECSFunctions[hashCode] = setActiveEcsFunction;
			m_ComponentSizes[hashCode] = sizeof(T);
			if (shouldHideInInspectorUI) {
				m_ShouldHideInInspectorUI.insert(hashCode);
			}
			m_SerialiseFunctions[hashCode] = [serialiseFunction](Json& json, const void* value) { serialiseFunction(json, *reinterpret_cast<const T*>(value)); };
			m_DeserialiseFunctions[typeId.Name()] = [deserialiseFunction](ECS& ecs, EntityId entity, const Json& json) {
				ecs.SetComponent<T>(entity, deserialiseFunction(json));
				};
		}

		void RegisterComponentsInECS(ECS& ecs);

	private:
		std::unordered_map<size_t, std::function<void(EntityId, void*)>> m_RenderEditorUIFunctions; // type hash code -> function expecting T* which renders inspector ui
		std::unordered_map<size_t, std::function<void(ECS&)>> m_SetActiveECSFunctions; // type hash code -> function which sets active ecs
		std::unordered_map<size_t, size_t> m_ComponentSizes; // type hash code -> sizeof(T)
		std::unordered_set<size_t> m_ShouldHideInInspectorUI; // type hash code
		std::unordered_map<size_t, std::function<void(Json&, const void*)>> m_SerialiseFunctions; // type hash code -> function
		std::unordered_map<std::string, std::function<void(ECS&, EntityId, const Json&)>> m_DeserialiseFunctions; // type hash code -> function
		std::vector<std::function<void(ECS&)>> m_ECSRegisterFunctions;

		std::unordered_set<std::type_index> RegisteredComponents;
	};

	// Registers the component
	template <typename T>
	class ComponentRegister {
		DISABLE_COPY_AND_MOVE(ComponentRegister);
	public:
		virtual void RenderInspectorUI(EntityId id, T& comp) {
			ImGui::PushTextWrapPos();
			ImGui::Text("Override RenderInspectorUI in your ComponentRegister<T> to add UI here.");
			ImGui::PopTextWrapPos();
		};

		virtual T Deserialise(const Json& json) {
			assert(false);
			return {};
		}

		virtual void Serialise(Json& json, const T& comp) {
			assert(false);
		}

	public:
		// Register a component
		ComponentRegister(
			bool shouldHideInInspectorUI = false
		) {
			std::type_index typeIndex(typeid(T));
			if (ComponentRegistry::Instance().RegisteredComponents.contains(typeIndex)) return;
			ComponentRegistry::Instance().RegisteredComponents.insert(typeIndex);

			const std::function<void(EntityId, T&)> renderInspectorUiFunction = [this](EntityId id, T& comp) { RenderInspectorUI(id, comp); };
			const std::function<void(ECS&)> setEcsFunction = [this](ECS& ecs) { m_ECS = ecs; };
			const std::function<void(Json&, const T&)> serialiseFunction = [this](Json& json, const T& comp) { Serialise(json, comp); };
			const std::function<T(const Json&)> deserialiseFunction = [this](const Json& json) -> T { return Deserialise(json); };

			ComponentRegistry::Instance().RegisterComponent(
				shouldHideInInspectorUI,
				renderInspectorUiFunction,
				setEcsFunction,
				serialiseFunction,
				deserialiseFunction
			);
		}

	protected:
		ECS& GetECS() { return m_ECS.Get(); }

	private:
		Reference<ECS> m_ECS; // Works like a state machine
	};
}

#define COMPONENT_REGISTER(Register) \
		inline static Register ComponentRegister_##Register