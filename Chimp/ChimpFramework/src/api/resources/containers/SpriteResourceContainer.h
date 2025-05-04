#pragma once

#include "api/resources/ResourceContainer.h"
#include "api/graphics/meshes/Mesh.h"
#include "Loggers.h"

namespace Chimp {
	class Engine;
	class SpriteResourceContainer : public ResourceContainer<TextureResourcePath, Mesh> {
	public:
		SpriteResourceContainer(Engine& engine, ResourceContainer<TextureResourcePath, ITexture>& textures) :
			ResourceContainer<TextureResourcePath, Mesh>([&](const TextureResourcePath& paths) { return LoadResource(engine, paths); }),
			m_Textures(textures) {

		}

		~SpriteResourceContainer() = default;

	private:
		Resource<Mesh> LoadResource(Engine& engine, const TextureResourcePath& paths);
		ResourceContainer<TextureResourcePath, ITexture>& m_Textures;
	};
}