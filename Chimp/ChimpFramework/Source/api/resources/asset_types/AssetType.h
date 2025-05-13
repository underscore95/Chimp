#pragma once

#include "stdafx.h"

namespace Chimp {
	enum class AssetImportState {
		Imported, NeedsReimporting, NotImported
	};

	class AssetType {
	protected:
		AssetType(const std::string& name) : m_Name(name) {}

	public:
		virtual ~AssetType() = default;

	public:
		// Return true if the file extension is this asset, e.g .obj returns true for model asset type
		virtual bool IsThisAssetType(const std::string& fileExtension) = 0;

		// Render import options
		virtual AssetImportState RenderImportOptionsUI(const std::filesystem::path& assetPath) = 0;

		// Import the asset
		virtual	void ImportAsset(const std::filesystem::path& assetPath) = 0;

		// Unimport the asset
		virtual	void UnimportAsset(const std::filesystem::path& assetPath) = 0;

		// Get all imported assets
		virtual const std::vector<std::filesystem::path>& GetImportedAssets() const = 0;

		// Is asset imported
		virtual	bool IsImported(const std::filesystem::path& assetPath) const = 0;

		std::string_view GetName() { return m_Name; }

	private:
		std::string m_Name;
	};
}