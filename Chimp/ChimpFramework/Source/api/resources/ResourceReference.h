#pragma once

#include "stdafx.h"
#include "api/utils/OptionalReference.h"
#include "Loggers.h"

namespace Chimp {

	class ImportedAssetsList {
	private:
		ImportedAssetsList() = default;
	public:
		static ImportedAssetsList& Instance();

		void NotifyAssetLoaded(const std::filesystem::path& path);
		void NotifyAssetUnloaded(const std::filesystem::path& path);

		int GetAssetIndex(const std::filesystem::path& path);
		bool IsAssetImported(int index);
		std::filesystem::path GetPath(int index);
	private:
		std::vector<bool> m_AssetStillImported;
		std::unordered_map<std::filesystem::path, int> m_AssetIndices;
	};

	template <typename T>
	class ResourceReference {
	public:
		ResourceReference() : Ref(nullptr)
#ifndef NDEBUG
			, AssetIndex(-1)
#endif
		{}
		ResourceReference(Reference<T> ref, const std::filesystem::path& path) :
			Ref(ref)
#ifndef NDEBUG
			, AssetIndex(ImportedAssetsList::Instance().GetAssetIndex(path))
#endif
		{}

		OptionalReference<T> Get() {
#ifndef NDEBUG
			if (!ImportedAssetsList::Instance().IsAssetImported(AssetIndex)) {
				Loggers::Resources().Warning(std::format(
					"Reference to asset with index {} at address {} with path {} no longer exists."),
					AssetIndex, Ref.GetNullablePtr(), ImportedAssetsList::Instance().GetPath(AssetIndex)
				);
				Ref = nullptr;
			}
#endif
			return Ref;
		}

		void Set(Reference<T> ref, const std::filesystem::path& path) {
			Ref = ref;
#ifndef NDEBUG
			AssetIndex = ImportedAssetsList::Instance().GetAssetIndex(path);
#endif
		}

	private:
		Reference<T> Ref;
#ifndef NDEBUG
			int AssetIndex;
#endif
	};
}