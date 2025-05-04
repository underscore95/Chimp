#include "ImageLoader.h"
#include "Loggers.h"

namespace Chimp::STB {
	ImageLoader::LoadedImage::~LoadedImage()
	{
		stbi_image_free(Data);
		Data = nullptr;
	}

	std::unique_ptr<IImageLoader::LoadedImage> ImageLoader::LoadImage(const std::string& path, const Options& options)
	{
		std::unique_ptr<LoadedImage> image = std::make_unique<LoadedImage>();

		stbi_set_flip_vertically_on_load(options.FlipVertically);

		image->Data = stbi_load(path.c_str(), &image->Width, &image->Height, &image->NumberChannels, 0);

#ifndef NDEBUG
		if (!image->IsValid())
		{
			Loggers::Resources().Error("Failed to load image! Path:" + path + " - Fail Reason: " + stbi_failure_reason());
		}
		else {
			Loggers::Resources().Info("--------------------------");
			Loggers::Resources().Info("Loaded image: " + path);
			Loggers::Resources().Info("Width: " + std::to_string(image->Width));
			Loggers::Resources().Info("Height: " + std::to_string(image->Height));
			Loggers::Resources().Info("Number of channels: " + std::to_string(image->NumberChannels));
			Loggers::Resources().Info("--------------------------");
		}
#endif

		return image;
	}
}