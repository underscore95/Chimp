#include "api/graphics/IRenderingManager.h"

namespace Chimp {
	IRenderingManager::IRenderingManager(IImageLoader& imageLoader) :
		m_ImageLoader(imageLoader)
	{
	}

	std::unique_ptr<IBuffer> IRenderingManager::CreateBuffer(const size_t size, const size_t numElements, const Usage& usage, const BindTarget target) const
	{
		auto buffer = CreateBuffer(usage, target);
		RawArray data;
		data.Data = new char[size];
		data.Size = size;
		data.NumberElements = numElements;
		buffer->SetData(data);
		return std::move(buffer);
	}

	std::unique_ptr<IImageLoader::LoadedImage> IRenderingManager::LoadImage(const std::string& filePath) const
	{
		return m_ImageLoader.LoadImage(filePath);
	}

	std::unique_ptr<ITexture> IRenderingManager::CreateCustomisedTextureFromImage(
		const TextureSlot slot,
		const TextureProperties& properties,
		const std::shared_ptr<IImageLoader::LoadedImage> image) const
	{
		return CreateTexture(slot, properties, image->Data);
	}

	std::unique_ptr<ITexture> IRenderingManager::CreateTextureFromImage(
		const std::string& filePath,
		const TextureProperties& properties
		) const
	{
		// Load image
		std::unique_ptr<IImageLoader::LoadedImage> image = m_ImageLoader.LoadImage(filePath);
		if (!image->IsValid())
		{
			return nullptr;
		}

		// Set properties
		TextureProperties thisTextureProperties = properties;
		thisTextureProperties.Width = image->Width;
		thisTextureProperties.Height = image->Height;
		thisTextureProperties.NumberOfChannels = image->NumberChannels;

		// Create texture
		return CreateCustomisedTextureFromImage(CHIMP_TEXTURE_SLOT, thisTextureProperties, std::move(image));
	}

}