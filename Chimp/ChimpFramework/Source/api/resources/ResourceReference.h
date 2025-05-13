#pragma once

#include "stdafx.h"
#include "api/utils/OptionalReference.h"
#include "Loggers.h"
#include "api/utils/AnyReference.h"

namespace Chimp {

	class IShader;
	class GameShader;

	class ImportedAssetsList {
	private:
		ImportedAssetsList() = default;
	public:
		static ImportedAssetsList& Instance();

		void NotifyAssetLoaded(const std::filesystem::path& path, AnyReference ref);
		void NotifyAssetUnloaded(const std::filesystem::path& path);

		int GetAssetIndex(const std::filesystem::path& path);
		bool IsAssetImported(int index);
		std::filesystem::path GetPath(int index);

		template <typename T>
		Reference<T> GetAsset(int index) {
#ifndef NDEBUG
			auto it = m_AssetLocations.find(GetPath(index));
			if (it == m_AssetLocations.end()) return nullptr;
			void* data = it->second.GetPtr();
			assert(it->second.GetType() == typeid(T));
			return static_cast<T*>(data);
#else
			return nullptr;
#endif
		}
	private:
		std::vector<bool> m_AssetStillImported;
		std::unordered_map<std::filesystem::path, int> m_AssetIndices;
		std::unordered_map<std::filesystem::path, AnyReference> m_AssetLocations;
	};

	template <typename T>
	class ResourceReference {
		static_assert(
			!std::is_same_v<T, IShader> && !std::is_same_v<T, GameShader>,
			"T must not be IShader or GameShader"
			);
	public:

		ResourceReference() : Ref(nullptr)
#ifndef NDEBUG
			, AssetIndex(-1)
#endif
		{
		}
		ResourceReference(Reference<T> ref, const std::filesystem::path& path) :
			Ref(ref)
#ifndef NDEBUG
			, AssetIndex(ImportedAssetsList::Instance().GetAssetIndex(path))
#endif
		{
		}

		OptionalReference<T> Get() {
#ifndef NDEBUG
			if (!ImportedAssetsList::Instance().IsAssetImported(AssetIndex)) {
				if (CachedAssetValid) {
					// Our asset was unloaded and we don't know about it yet
					Loggers::Resources().Warning(std::format(
						"Reference to asset with index {} at address {} with path {} no longer exists.",
						AssetIndex, static_cast<void*>(Ref.GetNullablePtr()), ImportedAssetsList::Instance().GetPath(AssetIndex).string()
					));
					CachedAssetValid = false;
				}
			}
			else if (!CachedAssetValid) {
				// Our asset was unloaded but then reloaded and we don't know about it yet
				CachedAssetValid = true;
				Ref = ImportedAssetsList::Instance().GetAsset<T>(AssetIndex);
			}
			return CachedAssetValid ? Ref : nullptr;
#else
			return Ref;
#endif
		}

		void Set(Reference<T> ref, const std::filesystem::path& path) {
			Ref = ref;
#ifndef NDEBUG
			AssetIndex = ImportedAssetsList::Instance().GetAssetIndex(path);
#endif
		}

		operator bool() const {
			return Ref
#ifndef NDEBUG
				&& ImportedAssetsList::Instance().IsAssetImported(AssetIndex)
#endif
				;
		}

	private:
		Reference<T> Ref;
#ifndef NDEBUG
		int AssetIndex;
		bool CachedAssetValid = true;
#endif
	};
}