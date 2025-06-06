#include "InspectorScript.h"
#include "scene_hierarchy\SceneHierarchyScript.h"

namespace ChimpEditor {
	InspectorScript::InspectorScript(Chimp::Engine& engine,
		Chimp::ECS& ecs,
		Chimp::ECS& gameEcs) :
		IEntityScript(typeid(this).name(), engine, ecs),
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
		ImGui::SetNextWindowSize(
			Chimp::ComponentMultiply(GetEngine().GetWindow().GetSize(), { 0.25f, 1.0f }) - Chimp::Vector2f{ 8,0 }
		);
		ImGui::Begin("Inspector");

		if (m_sceneHierarchy->HasSelectedEntity()) {
			// Entity name
			auto ent = m_sceneHierarchy->GetSelectedEntity();
			std::string str = std::to_string(ent);
			auto nameComp = m_gameEcs.GetComponent<Chimp::EntityNameComponent>(ent);
			ImGui::Text(nameComp ? nameComp->Name.c_str() : str.c_str());

			// Options
			RenderByteVisualisationOption();

			// Add / remove components
			RenderAddRemoveComponentButtons();

			// Components
			m_gameEcs.GetComponentsOnEntity(ent, [this, ent](Chimp::AnyReference component) {
				if (Chimp::ComponentRegistry::Instance().ShouldHideInInspectorUI(component.GetType())) return;

				auto name = GetComponentTypeName(component);
				if (ImGui::CollapsingHeader(name.c_str())) {
					RenderByteVisualisation(component);
					Chimp::ComponentRegistry::Instance().RenderEditorUI(m_gameEcs, ent, component);
				}
				});
		}

		ImGui::End();
	}

	void InspectorScript::RenderByteVisualisationOption()
	{
		const char* visualisationFormats[] = { "None", "Hex", "Binary" };
		int currentSelection = static_cast<int>(m_byteVisualisationFormat);

		if (ImGui::BeginCombo("Byte Visualisation", visualisationFormats[currentSelection], ImGuiComboFlags_WidthFitPreview))
		{
			for (int i = 0; i < IM_ARRAYSIZE(visualisationFormats); i++)
			{
				bool isSelected = (currentSelection == i);
				if (ImGui::Selectable(visualisationFormats[i], isSelected))
				{
					currentSelection = i;
					m_byteVisualisationFormat = static_cast<ByteVisualisationFormat>(i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

	void InspectorScript::RenderByteVisualisation(Chimp::AnyReference component)
	{
		if (m_byteVisualisationFormat == ByteVisualisationFormat::None) return;

		std::string bytes;
		if (m_byteVisualisationFormat == ByteVisualisationFormat::Hex) {
			bytes = "Data: " + Chimp::ByteVisualiser::VisualiseBytesHex(
				(char*)component.GetPtr(),
				Chimp::ComponentRegistry::Instance().GetSize(component)
			);
		}
		else if (m_byteVisualisationFormat == ByteVisualisationFormat::Binary) {
			bytes = "Data: " + Chimp::ByteVisualiser::VisualiseBytesBinary(
				(char*)component.GetPtr(),
				Chimp::ComponentRegistry::Instance().GetSize(component)
			);
		}

		ImGui::PushTextWrapPos();
		ImGui::TextWrapped("%s", bytes.c_str());
		ImGui::PopTextWrapPos();
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

	void InspectorScript::RenderAddRemoveComponentButtons()
	{
		// Add component
		if (m_selectedComponentAdd.Empty()) ImGui::BeginDisabled();
		if (ImGui::Button("Add Component:")) {
			m_gameEcs.AddDefaultComponent(m_sceneHierarchy->GetSelectedEntity(), m_selectedComponentAdd.UnsafeGet());
			m_selectedComponentAdd.Reset();
			// handle euler rotation component
			m_sceneHierarchy->SelectEntity(m_sceneHierarchy->GetSelectedEntity());
		}
		else if (m_selectedComponentAdd.Empty()) ImGui::EndDisabled();
		RenderComponentDropdown("##add-comp", m_selectedComponentAdd, [this](Chimp::TypeInfo typeInfo) {
			return !m_gameEcs.HasComponent(m_sceneHierarchy->GetSelectedEntity(), typeInfo);
			});

		// Remove component
		if (m_selectedComponentRemove.Empty()) ImGui::BeginDisabled();
		if (ImGui::Button("Remove Component:")) {
			m_gameEcs.RemoveComponent(m_sceneHierarchy->GetSelectedEntity(), m_selectedComponentRemove.UnsafeGet());
			m_selectedComponentRemove.Reset();
			// handle euler rotation component
			m_sceneHierarchy->SelectEntity(m_sceneHierarchy->GetSelectedEntity());
		}
		else if (m_selectedComponentRemove.Empty()) ImGui::EndDisabled();
		RenderComponentDropdown("##remove-comp", m_selectedComponentRemove, [this](Chimp::TypeInfo typeInfo) {
			return m_gameEcs.HasComponent(m_sceneHierarchy->GetSelectedEntity(), typeInfo) && Chimp::ComponentRegistry::Instance().IsAllowedToRemove(typeInfo);
			});

	}
	void InspectorScript::RenderComponentDropdown(const char* dropdownLabel, Chimp::InPlaceOptional<Chimp::TypeInfo>& storage, const std::function<bool(Chimp::TypeInfo)>& predicate)
	{
		if (ImGui::BeginCombo(dropdownLabel, storage.Empty() ? "None" : storage.UnsafeGet().Name())) {
			for (Chimp::TypeInfo comp : Chimp::ComponentRegistry::Instance().GetRegisteredTypes()) {
				if (
					!predicate(comp) ||
					Chimp::ComponentRegistry::Instance().ShouldHideInInspectorUI(comp)) {
					if (storage == comp) storage.Reset();
					continue;
				}

				if (ImGui::Selectable(comp.Name(), storage.HasValue() && storage == comp)) {
					storage = comp;
				}
			}

			ImGui::EndCombo();
		}
	}
}