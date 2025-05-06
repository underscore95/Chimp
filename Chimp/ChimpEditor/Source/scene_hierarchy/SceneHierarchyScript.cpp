#include "SceneHierarchyScript.h"
#include "EntityNameComponent.h"

namespace ChimpEditor {
	SceneHierarchyScript::SceneHierarchyScript(Chimp::EntityId entity,
		Chimp::Engine& engine,
		Chimp::ECS& ecs,
		Chimp::ECS& gameECS) :
		IEntityScript(entity, engine, ecs),
		m_gameECS(gameECS)
	{
	}

	void SceneHierarchyScript::OnInit()
	{
	}

	void SceneHierarchyScript::OnUpdate()
	{
	}

	void SceneHierarchyScript::OnRender()
	{
	}

	void SceneHierarchyScript::OnRenderUI()
	{
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSizeConstraints(
			{ 16, 16 },
			Chimp::ComponentMultiply(GetEngine().GetWindow().GetSize(), { 0.25f, 1.0f }) - Chimp::Vector2f{ 8,0 }
		);
		ImGui::Begin("Scene Hierarchy");

		auto view = m_gameECS.GetEntitiesWithComponents<Chimp::EntityIdComponent, Chimp::HierarchyComponent>();

		for (auto& [entityId, hierarchy] : view) {
			if (hierarchy.HierarchyLevel > 0) continue;
				RenderUI(entityId.Id, hierarchy, ImGui::GetCursorPosX());
		}

		ImGui::End();
	}

	void SceneHierarchyScript::RenderUI(Chimp::EntityId entity, Chimp::HierarchyComponent& hierarchyComp, float cursorPosX)
	{
		float oldCursorPosX = ImGui::GetCursorPosX();
		ImGui::SetCursorPosX(cursorPosX);

		// Get entity name
		auto nameComp = m_gameECS.GetComponent<EntityNameComponent>(entity);
		auto entityIdString = entity.str();
		auto entityName = nameComp ? nameComp->Name.c_str() : entityIdString.c_str();
		
		// Prevent opening if has no children
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		if (hierarchyComp.Children.Size() <= 0) flags |= ImGuiTreeNodeFlags_Leaf;

		// Draw ui
		if (ImGui::CollapsingHeader(entityName, flags)) {

			// Recurse children
			for (auto& child : hierarchyComp.Children) {
				RenderUI(child, m_gameECS.GetMutableComponent<Chimp::HierarchyComponent>(child).Get(), cursorPosX + 8);
			}
		}

		// Reset cursor
		ImGui::SetCursorPosX(oldCursorPosX);
	}
}