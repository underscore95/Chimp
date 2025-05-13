#pragma once

#include "api/resources/containers/TextureResourceContainer.h"
#include "api/Engine.h"
#include "api/graphics/textures/ITexture.h"

namespace Chimp {
	TextureResourceContainer::TextureResourceContainer(Engine& engine) :
		ResourceContainer([&](const TextureResourcePath& path) { return LoadResource(engine, path); })
	{
	}
	Resource<ITexture> TextureResourceContainer::LoadResource(Engine& engine, const TextureResourcePath& path)
	{
		assert(!path.empty());
		return Resource<ITexture>(engine.GetRenderingManager().CreateTextureFromImage(path));
	}
}