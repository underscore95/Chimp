#pragma once

#include "stdafx.h"
#include "buffers/Usage.h"
#include "buffers/IBuffer.h"
#include "buffers/IElementArrayLayout.h"
#include "buffers/IElementArray.h"
#include "shaders/IShader.h"
#include "shaders/ShaderTypes.h"
#include "PrimitiveType.h"
#include "IRenderer.h"
#include "api/graphics/images/IImageLoader.h"
#include "api/graphics/textures/ITexture.h"
#include "api/graphics/shaders/shaders/ChimpShaders.h"
#include "api/graphics/utils/IRenderTexture.h"
#include "api/graphics/utils/shadows/ICubeShadowMap.h"

namespace Chimp {
	class Engine;
	class IRenderingManager {
		friend class Engine;
	protected:
		IRenderingManager(Engine* engine, IImageLoader& imageLoader);

	public:
		~IRenderingManager() = default;

	private:
		void InitChimpShaders(Engine& engine);
		void DestroyChimpShaders() {
			m_ChimpShaders = nullptr;
		}

	public:
		static constexpr TextureSlot CHIMP_TEXTURE_SLOT = 0;

	public:

		// Get the renderer
		[[nodiscard]] virtual IRenderer& GetRenderer() const = 0;

		// Create a buffer which holds data on the GPU.
		// size, numElements can be left empty and will be updated once data is set.
		// size - the size of the buffer in bytes (default 0)
		// numElements - the number of elements in the buffer (default 0)
		// usage - defines if the buffer can be updated and how often it will be updated (hint for optimization)
		// target - defines how the buffer will be used (e.g. vertex buffer, index buffer, etc.)
		[[nodiscard]] virtual std::unique_ptr<IBuffer> CreateBuffer(
			const size_t size,
			const size_t numElements,
			const Usage& usage,
			const BindTarget target
		) const;
		[[nodiscard]] virtual std::unique_ptr<IBuffer> CreateBuffer(
			const Usage& usage,
			const BindTarget target
		) const = 0;

		// Create a render texture
		// width - The width in pixels
		// height - The height in pixels
		[[nodiscard]] virtual std::unique_ptr<IRenderTexture> CreateRenderTexture(
			unsigned int width,
			unsigned int height
		) const = 0;

		// Create a shadow map
		// width - The width in pixels
		// height - The height in pixels
		// numLights - Maximum number of lights
		[[nodiscard]] virtual std::unique_ptr<IRenderTexture> CreateShadowMap(
			unsigned int width,
			unsigned int height,
			unsigned int numLights
		) const = 0;

		// Create a cube map
		// size - The width and height in pixels
		// numLights - Maximum number of lights
		[[nodiscard]] virtual std::unique_ptr<ICubeShadowMap> CreateCubeShadowMap(
			unsigned int size,
			unsigned int numLights
		) const = 0;

		// Create an element array layout which defines how the data in an ElementArray is structured
		// primitivesType - the type of primitives in the ElementArray
		// layouts - a vector of ElementComponentLayouts which define the layout of each component of data in the ElementArray
		[[nodiscard]] virtual std::unique_ptr<IElementArrayLayout> CreateElementArrayLayout(
			const PrimitiveType primitivesType,
			const std::vector<ElementComponentLayout>& layouts) const = 0;

		// Create an element array which contains a vertex and index buffer.
		// vertexBuffer - the buffer which contains the vertex data
		// indexBuffer - the buffer which contains the index data
		// indexType - the type of data in the index buffer
		// layout - describes the layout of the vertex data
		[[nodiscard]] virtual std::unique_ptr<IElementArray> CreateElementArray(
			std::shared_ptr<IBuffer> vertexBuffer,
			std::unique_ptr<IBuffer> indexBuffer,
			GraphicsType indexType,
			std::shared_ptr<IElementArrayLayout> layout) const = 0;

		// Compile a shader from source code
		// In chimp, a "shader" represents all the shaders in the pipeline (vertex, fragment, etc.)
		// Check Shader::IsValid() to see if the shader was compiled successfully
		[[nodiscard]] virtual std::unique_ptr<IShader> CompileShader(
			const ShaderFilePaths& shaderFilePaths
		) const = 0;

		// Load an image from a file
		[[nodiscard]] virtual std::unique_ptr<IImageLoader::LoadedImage> LoadImage(
			const std::string& filePath
		) const;

		// Create a texture
		// The texture will be bound when it is constructed.
		// slot - the slot the texture is bound to
		// properties - the properties of the texture
		// initialData - the initial data to fill the texture with. This isn't owned by the texture, but it can be deleted after the constructor is called.
		[[nodiscard]] virtual std::unique_ptr<ITexture> CreateTexture(
			const TextureSlot slot,
			const TextureProperties& properties,
			const void* initialData) const = 0;

		// Create a texture from an image. Most use cases can use CreateTextureFromImage instead.
		// The image can be destroyed after this function call, for example by moving a unique ptr into this function.
		// slot - the slot the texture is bound to
		// properties - the properties of the texture, you should ensure properties like width, height, and number of channels match the image
		// image - the image to create the texture from (make sure it is valid!)
		[[nodiscard]] virtual std::unique_ptr<ITexture> CreateCustomisedTextureFromImage(
			const TextureSlot slot,
			const TextureProperties& properties,
			const std::shared_ptr<IImageLoader::LoadedImage> image) const;

		// Create a texture from an image.
		// The texture will be bound to CHIMP_TEXTURE_SLOT.
		// Relevant properties will be extracted from the image, however some texture properties like filtering must be set by you (or use default values).
		// This function will return a nullptr if the image is not loaded successfully.
		// filePath - the path to the image file
		// properties - the properties of the texture
		[[nodiscard]] virtual std::unique_ptr<ITexture> CreateTextureFromImage(
			const std::string& filePath,
			const TextureProperties& properties = {}
		) const;

		// Clear the depth buffer
		virtual void ClearDepthBuffer() const = 0;

		// Clear the depth buffer
		virtual void ClearColorBuffer() const = 0;

		// Set frame buffer
		virtual void SetFrameBuffer(int id = 0) const = 0;

		// Set the default render target, pass in nullptr to render to the back buffer
		void SetDefaultRenderTarget(std::weak_ptr<IRenderTexture> renderTarget);

		// Set render target to the default
		void BindDefaultRenderTarget();

		// Set view port
		virtual void SetViewport(Vector2i position, Vector2f size) const = 0;

		// Get shaders built into Chimp
		ChimpShaders& GetChimpShaders() const;

	protected:
		IImageLoader& m_ImageLoader;
		std::unique_ptr<ChimpShaders> m_ChimpShaders;
		std::weak_ptr<IRenderTexture> m_DefaultRenderTarget;
		Engine* m_Engine;
	};
}