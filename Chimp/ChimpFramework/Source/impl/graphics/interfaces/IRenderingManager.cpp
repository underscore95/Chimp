#include "api/graphics/IRenderingManager.h"
#include "api/Engine.h"

namespace Chimp {
	IRenderingManager::IRenderingManager(Reference<Engine> engine, IImageLoader& imageLoader) :
		m_ImageLoader(imageLoader),
		m_Engine(engine)
	{
	}

	void IRenderingManager::InitChimpShaders(Engine& engine)
	{
		m_ChimpShaders = std::unique_ptr<ChimpShaders>(new ChimpShaders(engine));
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

	void IRenderingManager::SetDefaultRenderTarget(std::weak_ptr<IRenderTexture> renderTarget)
	{
		m_DefaultRenderTarget = renderTarget;
	}

	void IRenderingManager::BindDefaultRenderTarget()
	{
		if (auto ptr = m_DefaultRenderTarget.lock()) {
			ptr->BindForWriting();
		}
		else {
			SetViewport({ 0,0 }, m_Engine->GetWindow().GetSize());
			SetFrameBuffer();
		}
	}

	ChimpShaders& IRenderingManager::GetChimpShaders() const
	{
		return *m_ChimpShaders;
	}

}