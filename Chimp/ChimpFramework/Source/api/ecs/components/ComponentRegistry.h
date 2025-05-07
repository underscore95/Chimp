#pragma once

#include "stdafx.h"

namespace Chimp {

	template <typename T>
	class ComponentRegister;

	class ComponentRegistry {
		template <typename T>
		friend class ComponentRegister;
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
			size_t hashCode = typeid(T).hash_code();
			m_RenderEditorUIFunctions[hashCode] = [function](void* value) { function(*reinterpret_cast<T*>(value)); }; // convert the function into one that takes a void*
		}


	private:
		std::unordered_map<size_t, std::function<void(void*)>> m_RenderEditorUIFunctions;
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
	};
}