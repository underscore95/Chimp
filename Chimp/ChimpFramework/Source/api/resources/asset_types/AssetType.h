#pragma once

#include "stdafx.h"

namespace Chimp {
	enum class AssetImportState {
		Imported, NeedsReimporting, NotImported
	};

	enum class AssetTypeId {
		Model
	};

	class AssetType {
	protected:
		AssetType(const std::string& name, AssetTypeId id) : 
			m_Name(name),
			m_Id(id)
		{}

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
		virtual std::vector<std::filesystem::path> GetImportedAssets() const = 0; // TODO return an iterator or something rather than a vector

		// Is asset imported
		virtual	bool IsImported(const std::filesystem::path& assetPath) const = 0;

		std::string_view GetName() const { return m_Name; }

		AssetTypeId GetId() const { return m_Id; }

	private:
		std::string m_Name;
		AssetTypeId m_Id;
	};
}