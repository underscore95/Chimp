#pragma once

#include "ResourceContainer.h"
#include "api/graphics/textures/ITexture.h"

namespace Chimp {
	// Depend on a resource when constructed, release it when destructed allowing the resource to be freed if it has 0 references.
	// Prefer using the ResourceContainer, as this is an immediate depend (increases max memory usage)
	template <typename K, typename T>
	class ResourceDependency {
	public:
		ResourceDependency(ResourceContainer<K, T>& resourceContainer, const K& key) :
			m_ResourceContainer(resourceContainer),
			m_Resource(m_ResourceContainer.ImmediateDepend(key)),
			m_Path(key)
		{
		}

		~ResourceDependency()
		{
			m_ResourceContainer.Release(m_Path);
		}

#pragma region copy
		ResourceDependency(const ResourceDependency& other) :
			m_ResourceContainer(other.m_ResourceContainer),
			m_Resource(m_ResourceContainer.ImmediateDepend(other.m_Path)),
			m_Path(other.m_Path)
		{
		}

		ResourceDependency& operator=(const ResourceDependency& other)
		{
			if (this == &other)
			{
				return *this;
			}

			m_ResourceContainer.Release(m_Path);
			m_ResourceContainer = other.m_ResourceContainer;
			m_Resource = m_ResourceContainer.ImmediateDepend(m_Path);
			m_Path = other.m_Path;

			return *this;
		}
#pragma endregion
#pragma region move
		// this is just the same as copying
		ResourceDependency(ResourceDependency&& other) noexcept :
			m_ResourceContainer(other.m_ResourceContainer),
			m_Resource(m_ResourceContainer.ImmediateDepend(other.m_Path)),
			m_Path(other.m_Path)
		{
		}

		ResourceDependency& operator=(ResourceDependency&& other)
		{
			if (this == &other)
			{
				return *this;
			}

			m_ResourceContainer.Release(m_Path);
			m_ResourceContainer = other.m_ResourceContainer;
			m_Resource = m_ResourceContainer.ImmediateDepend(m_Path);
			m_Path = other.m_Path;

			return *this;
		}
#pragma endregion

		T& GetResource() const
		{
			return m_Resource;
		}

		const K& GetPath() const
		{
			return m_Path;
		}

	private:
		ResourceContainer<K, T>& m_ResourceContainer;
		T& m_Resource;
		const K m_Path;
	};

	typedef ResourceDependency<TextureResourcePath, ITexture> TextureDependency;
}