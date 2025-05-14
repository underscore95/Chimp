#include "api/resources/containers/SpriteResourceContainer.h"
#include "api/graphics/meshes/TexturedQuad.h"
#include "api/Engine.h"
#include "api/graphics/IRenderingManager.h"
#include "api/resources/ResourceReference.h"

namespace Chimp {
	Resource<Mesh> SpriteResourceContainer::LoadResource(Engine& engine, const TextureResourcePath& path)
	{
		auto quad = TexturedQuad::Create(
			engine.GetRenderingManager(),
			TextureDependency{ m_Textures, path } // Immediate depend, this is fine since sprites themselves can be delayed if necessary
		);
		ImportedAssetsList::Instance().NotifyAssetLoaded(path, AnyReference{ *quad });
		return std::move(quad);
	}

	void SpriteResourceContainer::OnUnload(const TextureResourcePath& path)
	{
		ImportedAssetsList::Instance().NotifyAssetUnloaded(path);
	}
}