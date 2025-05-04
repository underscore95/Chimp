#include "api/resources/containers/SpriteResourceContainer.h"
#include "api/graphics/meshes/TexturedQuad.h"
#include "api/Engine.h"
#include "api/graphics/IRenderingManager.h"

namespace Chimp {
	Resource<Mesh> SpriteResourceContainer::LoadResource(Engine& engine, const TextureResourcePath& path)
	{
		return TexturedQuad::Create(
			engine.GetRenderingManager(),
			TextureDependency{ m_Textures, path } // Immediate depend, this is fine since sprites themselves can be delayed if necessary
		);
	}
}