#pragma once

#include "api/graphics/IRenderingManager.h"
#include "shaders/ShaderCompiler.h"
#include "impl/utils/NotNullHack.h"
#include "Renderer.h"

namespace Chimp::GL {
	class RenderingManager : public IRenderingManager {
	public:
		RenderingManager(Reference<Engine> engine, IImageLoader& imageLoader);
		~RenderingManager();

		IRenderer& GetRenderer() const override;

		void BeginDebugGroup(const std::string& name) const override;
		void EndDebugGroup() const override;

		std::unique_ptr<IBuffer> CreateBuffer(
			const Usage& usage,
			const BindTarget target
		) const override;

		[[nodiscard]] std::unique_ptr<IRenderTexture> CreateRenderTexture(
			unsigned int width,
			unsigned int height
		) const override;

		[[nodiscard]] std::unique_ptr<IRenderTexture> CreateShadowMap(
			unsigned int width,
			unsigned int height,
			unsigned int numLights
		) const override;

		[[nodiscard]] std::unique_ptr<ICubeShadowMap> CreateCubeShadowMap(
			unsigned int size,
			unsigned int numLights
		) const override;

		std::unique_ptr<IElementArrayLayout> CreateElementArrayLayout(
			const PrimitiveType primitivesType,
			const std::vector<ElementComponentLayout>& layouts
		) const override;

		std::unique_ptr<IElementArray> CreateElementArray(
			std::shared_ptr<IBuffer> vertexBuffer,
			std::unique_ptr<IBuffer> indexBuffer,
			GraphicsType indexType,
			std::shared_ptr<IElementArrayLayout> layout
		) const override;

		std::unique_ptr<IShader> CompileShader(const ShaderFilePaths& shaderFilePaths) const override;

		std::unique_ptr<ITexture> CreateTexture(
			const TextureSlot slot,
			const TextureProperties& properties,
			const void* initialData) const override;

		void ClearDepthBuffer() const override;

		void ClearColorBuffer() const override;

		void SetFrameBuffer(int id) const override;

		void SetViewport(Vector2i position, Vector2f size) const override;

	private:
		void InitOpenGL();

	private:
		NotNullHack<ShaderCompiler> m_ShaderCompiler;
		NotNullHack<Renderer> m_Renderer;
	};
}