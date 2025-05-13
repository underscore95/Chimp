#include "AssetImporterScript.h"
#include "AssetManagerScript.h"

namespace ChimpEditor {
	AssetImporterScript::AssetImporterScript(Chimp::EntityId entity,
		Chimp::Engine& engine,
		Chimp::ECS& ecs,
		AssetManagerScript& assetManager) :
		IEntityScript(entity, engine, ecs),
		m_assetManager(assetManager)
	{
	}

	void AssetImporterScript::OnInit()
	{
	}

	void AssetImporterScript::OnUpdate()
	{
	}

	void AssetImporterScript::OnRender()
	{
	}

	void AssetImporterScript::OnRenderUI()
	{
		if (!m_assetManager.HasFileSelected()) return;

		ImGui::SetNextWindowPos(
			Chimp::ComponentMultiply(GetEngine().GetWindow().GetSize(), { 0.75f, 0.5f })
		);
		ImGui::SetNextWindowSize(
			Chimp::ComponentMultiply(GetEngine().GetWindow().GetSize(), { 0.25f, 0.5f }) - Chimp::Vector2f{ 8,0 }
		);


		std::string fileExtension = Chimp::GetFileExtension(m_assetManager.GetSelectedFile());
		auto assetType = GetEngine().GetResourceManager().GetAssetTypeManager().GetType(fileExtension);
		if (assetType) {
			auto windowTitle = std::format("{} Asset Importer", assetType->GetName());
			ImGui::Begin(windowTitle.c_str());
			auto importState = assetType->RenderImportOptionsUI(m_assetManager.GetSelectedFile());
			RenderImportAssetButton(importState, assetType);
		}
		else {
			ImGui::Begin("Asset Importer");
			RenderInvalidUI();
		}

		ImGui::End();
	}

	void AssetImporterScript::RenderImportAssetButton(Chimp::AssetImportState state, Chimp::AssetType& assetType)
	{
		if (state == Chimp::AssetImportState::Imported) {
			ImGui::BeginDisabled();
			ImGui::Button("Asset Imported");
			ImGui::EndDisabled();
			return; 
		}

		if (!ImGui::Button(state == Chimp::AssetImportState::NeedsReimporting ? "Reimport Asset" : "Import Asset")) return;
		
		if (state == Chimp::AssetImportState::NeedsReimporting) {
			assetType.UnimportAsset(m_assetManager.GetSelectedFile());
		}		
		assetType.ImportAsset(m_assetManager.GetSelectedFile());
	}

	void AssetImporterScript::RenderInvalidUI()
	{
		ImGui::PushTextWrapPos();
		auto text = std::format("The file format '{}' is not supported.", Chimp::GetFileExtension(m_assetManager.GetSelectedFile()));
		ImGui::TextColored({ 1,0,0,1 }, text.c_str());
		ImGui::PopTextWrapPos();
	}
}