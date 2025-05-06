#pragma once

#include "stdafx.h"
#include "api/resources/ResourceContainer.h"
#include "api/graphics/images/IImageLoader.h"

namespace Chimp {
	// This is for loading images (cpu side) not textures (gpu side)
	// if you are rendering, you probably meant to get the texture resource container

	class Engine;

	class ImageResourceContainer : public ResourceContainer<ImageResourcePath, IImageLoader::LoadedImage> {
	public:
		ImageResourceContainer(Engine& engine);

	private:
		[[nodiscard]] Resource<IImageLoader::LoadedImage> LoadResource(Engine& engine, const ImageResourcePath& path);
	};
}