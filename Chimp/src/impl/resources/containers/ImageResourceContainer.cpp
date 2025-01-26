#pragma once

#include "api/resources/containers/ImageResourceContainer.h"
#include "api/Engine.h"
#include "api/graphics/textures/ITexture.h"

namespace Chimp {
	ImageResourceContainer::ImageResourceContainer(Engine& engine) :
		ResourceContainer([&](const ImageResourcePath& path) { return LoadResource(engine, path); })
	{
	}

	Resource<IImageLoader::LoadedImage> ImageResourceContainer::LoadResource(Engine& engine, const ImageResourcePath& path)
	{
		assert(!path.empty());
		return Resource<IImageLoader::LoadedImage>(engine.GetRenderingManager().LoadImage(path));
	}
}