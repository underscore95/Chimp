#include "api/resources/asset_types/AssetTypeManager.h"
#include "ModelAssetType.h"

namespace Chimp {
	AssetTypeManager::AssetTypeManager(Engine& engine)
	{
		RegisterAssetType(UNIQUE_PTR_CAST_FROM_RAW_PTR(AssetType, new ModelAssetType(engine)));
	}

	OptionalReference<AssetType> AssetTypeManager::GetType(const std::string& fileExtension)
	{
		for (auto& [name, type] : m_AssetTypes) {
			if (type->IsThisAssetType(fileExtension)) return type.get();
		}
		return nullptr;
	}

	void AssetTypeManager::RegisterAssetType(std::unique_ptr<AssetType> assetType)
	{
		assert(assetType);
		m_AssetTypes[std::string(assetType->GetName())] = std::move(assetType);
	}
}