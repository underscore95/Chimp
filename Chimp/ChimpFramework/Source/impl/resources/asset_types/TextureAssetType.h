#pragma once

#include "api/resources/asset_types/AssetType.h"

namespace Chimp {
	class Engine;
	class TextureAssetType : public AssetType {
		struct ImportOptions {
			bool GenerateSprite = true;

			bool operator==(const ImportOptions& other) const {
				return 
					GenerateSprite == other.GenerateSprite
					;
			}
		};
	public:
		TextureAssetType(Engine& engine);

		bool IsThisAssetType(const std::string& fileExtension) override;

		AssetImportState RenderImportOptionsUI(const std::filesystem::path& assetPath) override;

		void ImportAsset(const std::filesystem::path& assetPath) override;

		void UnimportAsset(const std::filesystem::path& assetPath) override;

		std::vector<std::filesystem::path> GetImportedAssets() const override;

		bool IsImported(const std::filesystem::path& assetPath) const override;

	private:
		std::unordered_map<std::filesystem::path, ImportOptions> m_ImportOptions;
		std::unordered_map<std::filesystem::path, ImportOptions> m_LastImportOptions;
		Engine& m_Engine;
	};
}