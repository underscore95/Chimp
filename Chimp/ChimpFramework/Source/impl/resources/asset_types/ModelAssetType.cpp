#include "ModelAssetType.h"
#include "Engine.h"
#include "api/resources/ResourceReference.h"

namespace Chimp {
	ModelAssetType::ModelAssetType(Engine& engine) :
		AssetType("Model", AssetTypeId::Model),
		m_Engine(engine)
	{
	}

	bool ModelAssetType::IsThisAssetType(const std::string& fileExtension)
	{
		return m_Engine.GetResourceManager().GetModels().GetModelImporter().IsSupportedFileExtension(fileExtension);
	}

	AssetImportState ModelAssetType::RenderImportOptionsUI(const std::filesystem::path& assetPath)
	{
		bool b;
		ImGui::Checkbox("Useless check box <3", &b);

		if (IsImported(assetPath)) {
			return AssetImportState::Imported;
		}
		return AssetImportState::NotImported;
	}

	void ModelAssetType::ImportAsset(const std::filesystem::path& assetPath)
	{
		assert(!IsImported(assetPath));

		auto& models = m_Engine.GetResourceManager().GetModels();
		models.ImmediateDepend(assetPath.string());
	}

	void ModelAssetType::UnimportAsset(const std::filesystem::path& assetPath)
	{
		assert(IsImported(assetPath));

		auto& models = m_Engine.GetResourceManager().GetModels();
		models.Release(assetPath.string());
		models.UnloadUnused();
	}

	std::vector<std::filesystem::path> ModelAssetType::GetImportedAssets() const
	{
		auto& models = m_Engine.GetResourceManager().GetModels();
		auto& sprites = m_Engine.GetResourceManager().GetSprites();

		std::vector<std::filesystem::path> paths;
		paths.reserve(models.GetLoadedResources().size() + sprites.GetLoadedResources().size());
		for (const auto& [path, _] : models.GetLoadedResources()) {
			paths.push_back(path);
		}
		for (const auto& [path, _] : sprites.GetLoadedResources()) {
			paths.push_back(path);
		}
		return paths;
	}

	bool ModelAssetType::IsImported(const std::filesystem::path& assetPath) const
	{
		return m_Engine.GetResourceManager().GetModels().IsLoaded(assetPath.string());
	}
}