#pragma once

#include "api/graphics/IRenderingManager.h"
#include "shaders/ShaderCompiler.h"
#include "impl/utils/NotNullHack.h"
#include "Renderer.h"

namespace Chimp::GL {
	class RenderingManager : public IRenderingManager {
	public:
		RenderingManager(IImageLoader& imageLoader);
		~RenderingManager();

		IRenderer& GetRenderer() const override;

		std::unique_ptr<IBuffer> CreateBuffer(
			const Usage& usage,
			const BindTarget target
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

	private:
		void InitOpenGL();

	private:
		NotNullHack<ShaderCompiler> m_ShaderCompiler;
		NotNullHack<Renderer> m_Renderer;
	};
}