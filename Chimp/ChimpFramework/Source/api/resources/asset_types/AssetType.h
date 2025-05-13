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
		virtual ~AssetType() {
			assert(m_ImportedAssets.empty()); // You forgot to unimport everything in your destructor
		}

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
		const std::vector<std::filesystem::path>& GetImportedAssets() const { return m_ImportedAssets; }

		// Is asset imported
		bool IsImported(const std::filesystem::path& assetPath) const { return std::find(m_ImportedAssets.begin(), m_ImportedAssets.end(), assetPath) != m_ImportedAssets.end(); }

		// Unimport all assets
		void UnimportAllAssets() {
			for (const auto& asset : m_ImportedAssets) UnimportAsset(asset);
			assert(m_ImportedAssets.empty());
		}

		std::string_view GetName() { return m_Name; }

	protected:
		std::vector<std::filesystem::path> m_ImportedAssets;

	private:
		std::string m_Name;
	};
}