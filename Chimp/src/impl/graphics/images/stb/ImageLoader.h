#pragma once

#include "api/graphics/images/IImageLoader.h"

namespace Chimp::STB {
	class ImageLoader : public IImageLoader {
	public:
		struct LoadedImage : public IImageLoader::LoadedImage {
			~LoadedImage() override;
		};

	public:
		std::unique_ptr<IImageLoader::LoadedImage> LoadImage(const std::string& path, const Options& options) override;
	};
}