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

		// Get all entities
		std::vector<Chimp::EntityId> ents = m_gameECS.GetEntities([this](Chimp::EntityId id) {
			auto hierarchy = m_gameECS.GetComponent<Chimp::HierarchyComponent>(id);
			assert(hierarchy);
			return hierarchy->HierarchyLevel == 0;
			});

		// Sort them by id to be consistent order
		std::sort(ents.begin(), ents.end());

		// Render ui
		for (Chimp::EntityId id : ents) {
			auto hierarchy = m_gameECS.GetComponent<Chimp::HierarchyComponent>(id);
			assert(hierarchy);
			if (hierarchy->HierarchyLevel > 0) continue; // Children get rendered recursively
			RenderUI(id, *hierarchy, ImGui::GetCursorPosX());
		}

		ImGui::End();
	}

	void SceneHierarchyScript::RenderUI(Chimp::EntityId entity, const Chimp::HierarchyComponent& hierarchyComp, float cursorPosX)
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