#pragma once

#include "stdafx.h"
#include "api/utils/OptionalReference.h"
#include "Loggers.h"
#include "api/utils/AnyReference.h"

namespace Chimp {

	class IShader;
	class GameShader;

	// TODO: What if something is imported as two different asset types? e.g a texture and a sprite
	class ImportedAssetsList {
	private:
		ImportedAssetsList() = default;
		~ImportedAssetsList();
	public:
		static ImportedAssetsList& Instance();

		void NotifyAssetLoaded(const std::filesystem::path& path, AnyReference ref);
		void NotifyAssetUnloaded(const std::filesystem::path& path);

#ifdef CHIMP_RESOURCE_SERIALISATION
		int GetAssetIndex(const std::filesystem::path& path);
		bool IsAssetImported(int index);
		std::filesystem::path GetPath(int index);
#endif

		template <typename T>
		Reference<T> GetAsset(int index) {
#ifdef CHIMP_RESOURCE_SERIALISATION
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

		ResourceReference() : m_Ref(nullptr)
#ifdef CHIMP_RESOURCE_SERIALISATION
			, m_AssetIndex(-1), m_CachedAssetValid(false)
#endif
		{
		}
		ResourceReference(OptionalReference<T> ref, const std::filesystem::path& path)
		{
			Set(ref, path);
		}

		OptionalReference<T> Get() {
#ifdef CHIMP_RESOURCE_SERIALISATION
			if (m_AssetIndex < 0) return nullptr;
			if (!ImportedAssetsList::Instance().IsAssetImported(m_AssetIndex)) {
				if (m_CachedAssetValid) {
					// Our asset was unloaded and we don't know about it yet
					Loggers::Resources().Warning(std::format(
						"Reference to asset with index {} at address {} with path {} no longer exists.",
						m_AssetIndex, static_cast<void*>(m_Ref.GetNullablePtr()), ImportedAssetsList::Instance().GetPath(m_AssetIndex).string()
					));
					m_CachedAssetValid = false;
				}
			}
			else if (!m_CachedAssetValid) {
				// Our asset was unloaded but then reloaded and we don't know about it yet
				m_CachedAssetValid = true;
				m_Ref = ImportedAssetsList::Instance().GetAsset<T>(m_AssetIndex);
			}
			return m_CachedAssetValid ? m_Ref : nullptr;
#else
			return m_Ref;
#endif
		}

		void Set(OptionalReference<T> ref, const std::filesystem::path& path) {

			assert(!(!ref && path != ""));
			assert(!(ref && path == ""));

			m_Ref = ref;
#ifdef CHIMP_RESOURCE_SERIALISATION
			m_AssetIndex = m_Ref ? ImportedAssetsList::Instance().GetAssetIndex(path) : -4;
			m_CachedAssetValid = false;
#endif
		}

		operator bool() const {
			return m_Ref
#ifdef CHIMP_RESOURCE_SERIALISATION
				&& ImportedAssetsList::Instance().IsAssetImported(m_AssetIndex)
#endif
				;
		}

#ifdef CHIMP_RESOURCE_SERIALISATION
		bool IsEqual(const std::filesystem::path& assetPath) const  {
			return  ImportedAssetsList::Instance().GetAssetIndex(assetPath) == m_AssetIndex && static_cast<bool>(*this);
		}

		std::filesystem::path GetPath() const  {
			return ImportedAssetsList::Instance().GetPath(m_AssetIndex);
		}

		int GetAssetIndex() const {
			return m_AssetIndex;
		}
#endif

	private:
		OptionalReference<T> m_Ref;
#ifdef CHIMP_RESOURCE_SERIALISATION
		int m_AssetIndex;
		bool m_CachedAssetValid;
#endif
	};
}