#include "ModelAssetType.h"
#include "Engine.h"

namespace Chimp {
	ModelAssetType::ModelAssetType(Engine& engine) :
		AssetType("Model"),
		m_Engine(engine)
	{
	}

	ModelAssetType::~ModelAssetType()
	{
		UnimportAllAssets();
	}

	bool ModelAssetType::IsThisAssetType(const std::string& fileExtension)
	{
		return m_Engine.GetResourceManager().GetModels().GetModelImporter().IsSupportedFileExtension(fileExtension);
	}

	AssetImportState ModelAssetType::RenderImportOptionsUI(const std::filesystem::path& assetPath)
	{
		bool b;
		ImGui::Checkbox("Useless check box <3", &b);

		if (m_Engine.GetResourceManager().GetModels().IsLoaded(assetPath.string())) {
			return AssetImportState::Imported;
		}
		return AssetImportState::NotImported;
	}

	void ModelAssetType::ImportAsset(const std::filesystem::path& assetPath)
	{
		assert(!IsImported(assetPath));

		auto& models = m_Engine.GetResourceManager().GetModels();
		models.ImmediateDepend(assetPath.string());

		m_ImportedAssets.push_back(assetPath);
	}

	void ModelAssetType::UnimportAsset(const std::filesystem::path& assetPath)
	{
		assert(IsImported(assetPath));

		auto& models = m_Engine.GetResourceManager().GetModels();
		models.Release(assetPath.string());

		m_ImportedAssets.erase(std::find(m_ImportedAssets.begin(), m_ImportedAssets.end(), assetPath));
	}
}