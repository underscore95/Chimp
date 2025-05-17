#include "api/ecs/components/ComponentRegistry.h"
#include "stdafx.h"

namespace Chimp {
	ComponentRegistry& Chimp::ComponentRegistry::Instance()
	{
		static ComponentRegistry inst;
		return inst;
	}

	void ComponentRegistry::RenderEditorUI(EntityId id, AnyReference value)
	{
		size_t hashCode = value.GetType().Hash();
		auto it = m_RenderEditorUIFunctions.find(hashCode);
		assert(it != m_RenderEditorUIFunctions.end());
		auto& func = it->second;
		void* voidValue = value.GetPtr();
		func(id, voidValue);
	}

	void ComponentRegistry::Serialise(Json& json, AnyReference component)
	{
		auto it = m_SerialiseFunctions.find(component.GetType().Hash());
		assert(it != m_SerialiseFunctions.end());
		Json compJson;
		it->second(compJson, component.GetPtr());

		json[component.GetType().Name()] = compJson;
	}

	void ComponentRegistry::Deserialise(ECS& ecs, EntityId entity, const std::string& typeName, const Json& json)
	{
		auto it = m_DeserialiseFunctions.find(typeName);
		assert(it != m_DeserialiseFunctions.end());

		const Json& compJson = json[typeName];
		it->second(ecs, entity, compJson);
	}

	void ComponentRegistry::RegisterComponentsInECS(ECS& ecs)
	{
		for (auto& func : m_ECSRegisterFunctions) {
			func(ecs);
		}
	}
}