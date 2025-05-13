#include "api/resources/ResourceReference.h"

namespace Chimp {
	ImportedAssetsList& Chimp::ImportedAssetsList::Instance()
	{
		static ImportedAssetsList instance;
		return instance;
	}

	void ImportedAssetsList::NotifyAssetLoaded(const std::filesystem::path& path, AnyReference ref)
	{
		assert(path != "");
		assert(ref.GetPtr() != nullptr);
#ifndef NDEBUG
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
		assert(path != "");
#ifndef NDEBUG
		m_AssetStillImported[m_AssetIndices[path]] = false;
		m_AssetLocations.erase(path);
#endif
	}

	int ImportedAssetsList::GetAssetIndex(const std::filesystem::path& path)
	{
#ifndef NDEBUG
		auto it = m_AssetIndices.find(path);
		if (it != m_AssetIndices.end()) { 
			return it->second;
		}
		return -3;
#else
		return -2;
#endif
	}

	bool ImportedAssetsList::IsAssetImported(int index)
	{
#ifndef NDEBUG
		return m_AssetStillImported[index];
#else
		return true;
#endif
	}

	std::filesystem::path ImportedAssetsList::GetPath(int index)
	{
		for (auto& [path, assetIndex] : m_AssetIndices) {
			if (index == assetIndex) return path;
		}
		return "";
	}
}