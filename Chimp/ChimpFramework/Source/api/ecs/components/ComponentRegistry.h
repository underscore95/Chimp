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
		void RenderEditorUI(AnyReference value) {
			size_t hashCode = value.GetType().Hash();
			auto it = m_RenderEditorUIFunctions.find(hashCode);
			assert(it != m_RenderEditorUIFunctions.end());
			auto& func = it->second;
			void* voidValue = value.GetPtr();
			func(voidValue);
		}

		size_t GetSize(AnyReference value) {
			size_t hashCode = value.GetType().Hash();
			auto it = m_ComponentSizes.find(hashCode);
			assert(it != m_ComponentSizes.end());
			return it->second;
		}

	private:
		// Register a component, this can only be called from ComponentRegister which ensures it is impossible to register a component twice
		template <typename T>
		void RegisterComponent(const std::function<void(T&)>& function) {
			const auto& typeId = typeid(T);
			size_t hashCode = typeId.hash_code();
			m_RenderEditorUIFunctions[hashCode] = [function](void* value) { function(*reinterpret_cast<T*>(value)); }; // convert the function into one that takes a void*
			m_ECSRegisterFunctions.push_back([](ECS& ecs) { ecs.RegisterComponent<T>(); });
			m_ComponentSizes[hashCode] = sizeof(T);
		}

		void RegisterComponentsInECS(ECS& ecs);

	private:
		std::unordered_map<size_t, std::function<void(void*)>> m_RenderEditorUIFunctions; // type hash code -> function expecting T* which renders inspector ui
		std::unordered_map<size_t, size_t> m_ComponentSizes; // type hash code -> sizeof(T)
		std::vector<std::function<void(ECS&)>> m_ECSRegisterFunctions;
	};

	// Registers the component
	template <typename T>
	class ComponentRegister {
		DISABLE_COPY_AND_MOVE(ComponentRegister);
	public:
		virtual void RenderInspectorUI(T& comp) {
			ImGui::PushTextWrapPos();
			ImGui::Text("Override RenderInspectorUI in your ComponentRegister<T> to add UI here.");
			ImGui::PopTextWrapPos();
		};

	public:
		// Register a component
		ComponentRegister() {
			static bool unused = [this]() {
				const std::function<void(T&)> renderInspectorUiFunction = [this](T& comp) { RenderInspectorUI(comp); };
				ComponentRegistry::Instance().RegisterComponent(
					renderInspectorUiFunction
				);

				return true;
				}();
		}
	};
}