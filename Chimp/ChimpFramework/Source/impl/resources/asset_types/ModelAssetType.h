#pragma once

#include "api/resources/asset_types/AssetType.h"

namespace Chimp {
	class Engine;
	class ModelAssetType : public AssetType {
	public:
		ModelAssetType(Engine& engine);
		~ModelAssetType() override;

		bool IsThisAssetType(const std::string& fileExtension) override;

		AssetImportState RenderImportOptionsUI(const std::filesystem::path& assetPath) override;

		void ImportAsset(const std::filesystem::path& assetPath) override;

		void UnimportAsset(const std::filesystem::path& assetPath) override;

	private:
		Engine& m_Engine;
	};
}