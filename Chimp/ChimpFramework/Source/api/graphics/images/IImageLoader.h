#pragma once

#include "stdafx.h"

namespace Chimp {
	class IImageLoader {
	public:
		// Represents a loaded image
		struct LoadedImage {
			friend class IImageLoader;
		protected:
			LoadedImage() = default;

		public:
			virtual ~LoadedImage(); // Deallocate the image data

			unsigned char* Data = nullptr; // The image data, owned by this struct
			int Width = 0; // The width of the image
			int Height = 0; // The height of the image
			int NumberChannels = 0; // The number of channels in the image (3 for RGB, 4 for RGBA)

			[[nodiscard]] bool IsValid() const;

			// Returns colour of pixel at position
			// if no alpha channel then alpha is 1
			// if no colour channel, then colour is 0
			// assumes row-major order (correct for STB)
			// returns def if out of bounds or invalid image
			// only supports 1/2/3/4 channels
			[[nodiscard]] Vector4f GetPixel(Vector2i pos, Vector4f def = {-1,-1,-1,-1});
		};

		// Represents options for loading an image
		struct Options {
			bool FlipVertically = false;
		};

	public:
		// Load an image from a file
		[[nodiscard]] virtual  std::unique_ptr<LoadedImage> LoadImage(
			const std::string& path,
			const Options& options = {}
		) = 0;
	};
}