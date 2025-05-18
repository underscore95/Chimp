#include "api/resources/ResourceReference.h"

namespace Chimp {
	ImportedAssetsList& Chimp::ImportedAssetsList::Instance()
	{
		static ImportedAssetsList instance;
		return instance;
	}

	void ImportedAssetsList::NotifyAssetLoaded(const std::filesystem::path& path, AnyReference ref)
	{
#ifdef CHIMP_RESOURCE_SERIALISATION
		assert(path != "");
		assert(ref.GetPtr() != nullptr);
		auto it = m_AssetIndices.find(path);
		if (it != m_AssetIndices.end()) {
			m_AssetStillImported[it->second] = true;
		}
		else {
			m_AssetStillImported.push_back(true);
			m_AssetIndices[path] = m_AssetStillImported.size() - 1;
		}
		m_AssetLocations[path] = ref;
#endif
	}

	void ImportedAssetsList::NotifyAssetUnloaded(const std::filesystem::path& path)
	{
#ifdef CHIMP_RESOURCE_SERIALISATION
		assert(path != "");
		m_AssetStillImported[m_AssetIndices[path]] = false;
		m_AssetLocations.erase(path);
#endif
	}

#ifdef CHIMP_RESOURCE_SERIALISATION
	int ImportedAssetsList::GetAssetIndex(const std::filesystem::path& path)
	{
		auto it = m_AssetIndices.find(path);
		if (it != m_AssetIndices.end()) { 
			return it->second;
		}
		return -3;

	}

	bool ImportedAssetsList::IsAssetImported(int index)
	{
		assert(index >= 0);
		return m_AssetStillImported[index];
	}

	std::filesystem::path ImportedAssetsList::GetPath(int index)
	{
		for (auto& [path, assetIndex] : m_AssetIndices) {
			if (index == assetIndex) return path;
		}
		return "";
	}

#endif
}