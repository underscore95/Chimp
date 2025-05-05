#pragma once

#include "api/graphics/images/IImageLoader.h"

namespace Chimp {

	Chimp::IImageLoader::LoadedImage::~LoadedImage()
	{
		assert(Data == nullptr); // This should have been deallocated by the derived class
	}

	bool Chimp::IImageLoader::LoadedImage::IsValid() const
	{
		return Data != nullptr;
	}

	Vector4f IImageLoader::LoadedImage::GetPixel(Vector2i pos, Vector4f def)
	{
		if (!IsValid() || pos.x < 0 || pos.y < 0 || pos.x >= Width || pos.y >= Height)
			return def;

		int index = (pos.y * Width + pos.x) * NumberChannels;
		if (NumberChannels == 1)
		{
			unsigned char r = Data[index];
			return { r / 255.0f, 0.0f, 0.0f, 1.0f };
		}
		else if (NumberChannels == 2)
		{
			unsigned char r = Data[index];
			unsigned char g = Data[index + 1];
			return { r / 255.0f, g / 255.0f, 0.0f, 1.0f };
		}
		else if (NumberChannels == 3)
		{
			unsigned char r = Data[index];
			unsigned char g = Data[index + 1];
			unsigned char b = Data[index + 2];
			return { r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
		}
		else if (NumberChannels == 4)
		{
			unsigned char r = Data[index];
			unsigned char g = Data[index + 1];
			unsigned char b = Data[index + 2];
			unsigned char a = Data[index + 3];
			return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		}
		else
		{
			assert(false);
			return def;
		}
	}
}