#include "SceneHierarchyScript.h"
#include "EntityNameComponent.h"
#include "api/ecs/hierarchy/HierarchyComponent.h"

namespace ChimpEditor {
	SceneHierarchyScript::SceneHierarchyScript(Chimp::EntityId entity,
		Chimp::Engine& engine,
		Chimp::ECS& ecs,
		Chimp::ECS& gameECS) :
		IEntityScript(entity, engine, ecs),
		m_gameECS(gameECS)
	{
	}

	Chimp::EntityId SceneHierarchyScript::GetSelectedEntity() const
	{
		return m_selectedEntity;
	}

	bool SceneHierarchyScript::HasSelectedEntity()
	{
		return m_selectedEntity != Chimp::EntityId{};
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

		// Draw UI
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (hierarchyComp.Children.Size() == 0) {
			// Leaf node
			if (ImGui::Selectable(entityName, m_selectedEntity == entity)) {
				m_selectedEntity = entity;
				GetLogger().Info("Selected ");
			}
		}
		else {
			// Node with children
			bool isSelected = (m_selectedEntity == entity);
			if (isSelected)
				nodeFlags |= ImGuiTreeNodeFlags_Selected;

			bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)entity, nodeFlags, entityName);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
				m_selectedEntity = entity;
				GetLogger().Info("Selected ");
			}

			if (nodeOpen) {
				for (auto& child : hierarchyComp.Children) {
					RenderUI(child, m_gameECS.GetMutableComponent<Chimp::HierarchyComponent>(child).Get(), cursorPosX + 8);
				}
				ImGui::TreePop();
			}
		}


		// Reset cursor
		ImGui::SetCursorPosX(oldCursorPosX);
	}
}