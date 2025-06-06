#include "TextureAssetType.h"
#include "Engine.h"
#include "api/resources/ResourceReference.h"

namespace Chimp {
	TextureAssetType::TextureAssetType(Engine& engine) :
		AssetType("Texture", AssetTypeId::Texture),
		m_Engine(engine)
	{
	}

	bool TextureAssetType::IsThisAssetType(const std::string& fileExtension)
	{
		return m_Engine.GetRenderingManager().SupportsImageFileExtension(fileExtension);
	}

	AssetImportState TextureAssetType::RenderImportOptionsUI(const std::filesystem::path& assetPath)
	{
		auto& options = m_ImportOptions[assetPath];

		ImGui::Checkbox("Generate Sprite Mesh", &options.GenerateSprite);

		if (IsImported(assetPath)) {
			auto it = m_LastImportOptions.find(assetPath);
			return it == m_LastImportOptions.end() || it->second != options ? AssetImportState::NeedsReimporting : AssetImportState::Imported;
		}
		return AssetImportState::NotImported;
	}

	void TextureAssetType::ImportAsset(const std::filesystem::path& assetPath)
	{
		assert(!IsImported(assetPath));

		// Import
		const auto& options = m_ImportOptions[assetPath];
		m_LastImportOptions[assetPath] = options;

		m_Engine.GetResourceManager().GetTextures().ImmediateDepend(assetPath.string());

		// Generate sprite
		if (options.GenerateSprite) {
			m_Engine.GetResourceManager().GetSprites().ImmediateDepend(assetPath.string());
		}
	}

	void TextureAssetType::UnimportAsset(const std::filesystem::path& assetPath)
	{
		assert(IsImported(assetPath));

		// If we generated a sprite previously, delete it
		{
			auto it = m_LastImportOptions.find(assetPath);
			if (it != m_LastImportOptions.end()) {
				const auto& lastOptions = it->second;
				if (lastOptions.GenerateSprite) {
					m_Engine.GetResourceManager().GetSprites().Release(assetPath.string());
					m_Engine.GetResourceManager().GetSprites().UnloadUnused();
				}
			}
		}

		auto& sprites = m_Engine.GetResourceManager().GetTextures();
		sprites.Release(assetPath.string());
		sprites.UnloadUnused();
	}

	std::vector<std::filesystem::path> TextureAssetType::GetImportedAssets() const
	{
		auto& sprites = m_Engine.GetResourceManager().GetTextures();
		std::vector<std::filesystem::path> paths;
		paths.reserve(sprites.GetLoadedResources().size());
		for (const auto& [path, _] : sprites.GetLoadedResources()) {
			paths.push_back(path);
		}
		return paths;
	}

	bool TextureAssetType::IsImported(const std::filesystem::path& assetPath) const
	{
		return m_Engine.GetResourceManager().GetTextures().IsLoaded(assetPath.string());
	}
}