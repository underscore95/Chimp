#pragma once

#include "Resource.h"
#include "ResourcePaths.h"

namespace Chimp {

	template <typename K, typename T>
	class ResourceContainer {
	public:
		ResourceContainer(std::function<Resource<T>(const K&)> loadResourceFunc) :
			m_LoadResourceFunc(loadResourceFunc)
		{
		}
		virtual ~ResourceContainer() = default;

		// Depend on a resource, but its not required right now
		const K& Depend(const K& path) {
			auto iter = m_Resources.find(path);
			if (iter == m_Resources.end()) {
				m_DelayedDependencies.insert(path); // Load it later
			}
			else {
				// Already loaded, depend on it
				iter->second.RefCount++;
			}
			return path;
		}

		// Immediate depend on a resource, this loads it if it hasn't been loaded yet and increments the reference count
		T& ImmediateDepend(const K& path) {
			auto iter = m_Resources.find(path);
			if (iter == m_Resources.end()) {
				auto resource = m_LoadResourceFunc(path);
				assert(resource.RefCount == 0);
				iter = m_Resources.emplace(path, std::move(resource)).first;
			}

			iter->second.RefCount++;
			return *iter->second.Data;
		}


		// Get a resource, does bad stuff if it doesn't exist, only call this if you are 100% sure it exists
		[[nodiscard]] T& Get(const K& path) {
			assert(IsLoaded(path));
			return *m_Resources[path].Data;
		}

		// Is a resource loaded?
		[[nodiscard]] bool IsLoaded(const K& path) {
			return m_Resources.find(path) != m_Resources.end();
		}

		// Release a resource, this decrements the reference count
		void Release(const K& path) {
			auto it = m_Resources.find(path);
			if (it == m_Resources.end()) {
				return;
			}

			it->second.RefCount--;
		}

		// Unload all resources that have a reference count of 0
		void UnloadUnused() {
			for (auto it = m_Resources.begin(); it != m_Resources.end();) {
				if (it->second.RefCount <= 0) {
					OnUnload(it->first);
					it = m_Resources.erase(it);
				}
				else {
					++it;
				}
			}
		}

		// Load all delayed dependencies
		void LoadDependencies() {
			for (const auto& path : m_DelayedDependencies) {
				ImmediateDepend(path);
			}
			m_DelayedDependencies.clear();
		}

		// Get number loaded resources
		[[nodiscard]] size_t Size() const {
			return m_Resources.size();
		}

		// Get loaded resources
		const std::unordered_map<K, Resource<T>>& GetLoadedResources() { return m_Resources; }

	protected:
		virtual void OnUnload(const K& path) {};

	protected:
		std::unordered_map<K, Resource<T>> m_Resources;

	private:
		std::function<Resource<T>(const K&)> m_LoadResourceFunc;
		std::unordered_set<K> m_DelayedDependencies;
	};
}