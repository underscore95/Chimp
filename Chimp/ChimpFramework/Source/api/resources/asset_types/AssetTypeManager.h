#pragma once

#include "stdafx.h"
#include "api/utils/OptionalReference.h"
#include "AssetType.h"

namespace Chimp {
	class Engine;

	class AssetTypeManager {
	public:
		AssetTypeManager(Engine& engine);

		OptionalReference<AssetType> GetType(const std::string& fileExtension);
		Reference<AssetType> GetType(AssetTypeId id);

	private:
		void RegisterAssetType(std::unique_ptr<AssetType> assetType);

	private:
		std::unordered_map<AssetTypeId, std::unique_ptr<AssetType>> m_AssetTypes;
	};
}