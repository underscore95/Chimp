#include "SceneHierarchyScript.h"
#include "api/ecs/hierarchy/HierarchyComponent.h"

namespace ChimpEditor {
	SceneHierarchyScript::SceneHierarchyScript(Chimp::Engine& engine,
		Chimp::ECS& ecs,
		Chimp::ECS& gameECS) :
		IEntityScript(typeid(this).name(), engine, ecs),
		m_gameECS(gameECS)
	{
	}

	Chimp::EntityId SceneHierarchyScript::GetSelectedEntity() const
	{
		return m_selectedEntity;
	}

	bool SceneHierarchyScript::HasSelectedEntity()
	{
		return m_hasSelectedEntity;
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
		ImGui::SetNextWindowSize(
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
		auto nameComp = m_gameECS.GetComponent<Chimp::EntityNameComponent>(entity);
		std::string entityName = nameComp ? nameComp->Name : std::format("Entity {}", entity);

		// Draw UI
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (hierarchyComp.Children.Size() == 0) {
			// Leaf node
			if (ImGui::Selectable(entityName.c_str(), m_selectedEntity == entity)) {
				SelectEntity(entity);
			}
		}
		else {
			// Node with children
			bool isSelected = (m_selectedEntity == entity);
			if (isSelected)
				nodeFlags |= ImGuiTreeNodeFlags_Selected;

			bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)entity, nodeFlags, entityName.c_str());
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
				SelectEntity(entity);
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

	void SceneHierarchyScript::SelectEntity(Chimp::EntityId entity)
	{
		m_hasSelectedEntity = true;
		m_selectedEntity = entity;
		auto transformComp = m_gameECS.GetComponent<Chimp::TransformComponent>(m_selectedEntity);
		if (transformComp && !m_gameECS.GetComponent<Chimp::EulerRotationComponent>(m_selectedEntity)) {
			m_gameECS.SetComponent<Chimp::EulerRotationComponent>(m_selectedEntity, { Chimp::ToEulerRotationDegrees(transformComp->LocalRotation) });
		}
	}
}