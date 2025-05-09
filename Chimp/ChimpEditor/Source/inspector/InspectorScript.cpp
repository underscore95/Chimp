#include "InspectorScript.h"
#include "scene_hierarchy\SceneHierarchyScript.h"
#include "scene_hierarchy\EntityNameComponent.h"

namespace ChimpEditor {
	InspectorScript::InspectorScript(Chimp::EntityId entity,
		Chimp::Engine& engine,
		Chimp::ECS& ecs,
		Chimp::ECS& gameEcs) :
		IEntityScript(entity, engine, ecs),
		m_gameEcs(gameEcs)
	{
	}

	void InspectorScript::OnInit()
	{
		auto ent = GetECS().GetScripts().GetFirstEntityWithScript<SceneHierarchyScript>();
		assert(ent);
		m_sceneHierarchy = ent->Script;
	}

	void InspectorScript::OnUpdate()
	{
	}

	void InspectorScript::OnRender()
	{
	}

	void InspectorScript::OnRenderUI()
	{
		ImGui::SetNextWindowPos({ GetEngine().GetWindow().GetSize().x * 0.75f + 8, 0 });
		ImGui::SetNextWindowSizeConstraints(
			{ 16, 16 },
			Chimp::ComponentMultiply(GetEngine().GetWindow().GetSize(), { 0.25f, 1.0f }) - Chimp::Vector2f{ 8,0 }
		);
		ImGui::Begin("Inspector");

		if (m_sceneHierarchy->HasSelectedEntity()) {
			auto ent = m_sceneHierarchy->GetSelectedEntity();
			auto str = ent.str();
			auto nameComp = m_gameEcs.GetComponent<EntityNameComponent>(ent);
			ImGui::Text(nameComp ? nameComp->Name.c_str() : str.c_str());

			m_gameEcs.GetComponentsOnEntity(ent, [this](Chimp::AnyReference component) {
				auto name = GetComponentTypeName(component);
				if (ImGui::CollapsingHeader(name.c_str())) {
					Chimp::ComponentRegistry::Instance().RenderEditorUI(component);
				}
				});
		}

		ImGui::End();
	}

	std::string InspectorScript::GetComponentTypeName(Chimp::AnyReference component)
	{
		std::string rawName = component.GetType().Name();
		if (rawName.starts_with("struct ")) {
			rawName.erase(0, 7);
		}
		else if (rawName.starts_with("class ")) {
			rawName.erase(0, 6);
		}
		return rawName;
	}
}