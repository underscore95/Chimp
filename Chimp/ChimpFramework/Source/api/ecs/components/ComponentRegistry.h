#pragma once

#include "stdafx.h"
#include "api/ecs/ECS.h"

namespace Chimp {

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
		template <typename T>
		void RenderEditorUI(T& value) {
			size_t hashCode = typeid(value).hash_code();
			auto it = m_RenderEditorUIFunctions.find(hashCode);
			if (it != m_RenderEditorUIFunctions.end()) {
				auto& func = it->second;
				void* voidValue = &value;
				func(voidValue);
			}
		}

	private:
		// Register a component, this can only be called from ComponentRegister which ensures it is impossible to register a component twice
		template <typename T>
		void RegisterComponent(const std::function<void(T&)>& function) {
			const auto& typeId = typeid(T);
			size_t hashCode = typeId.hash_code();
			m_RenderEditorUIFunctions[hashCode] = [function](void* value) { function(*reinterpret_cast<T*>(value)); }; // convert the function into one that takes a void*
			m_ECSRegisterFunctions.push_back([](ECS& ecs) { ecs.RegisterComponent<T>(); });
		}

		void RegisterComponentsInECS(ECS& ecs);

	private:
		std::unordered_map<size_t, std::function<void(void*)>> m_RenderEditorUIFunctions;
		std::vector<std::function<void(ECS&)>> m_ECSRegisterFunctions;
	};

	// Registers the component
	template <typename T>
	class ComponentRegister {
		DISABLE_COPY_AND_MOVE(ComponentRegister);
	public:
		// Register a component
		// function - function that should render any editor UI for the inspector
		ComponentRegister(const std::function<void(T&)>& func) {
			static bool unused = [func]() { ComponentRegistry::Instance().RegisterComponent(func); return true; }();
		}

		ComponentRegister() : ComponentRegister([](T&) {}) {}
	};
}