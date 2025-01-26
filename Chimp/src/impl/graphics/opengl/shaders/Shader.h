#pragma once

#include "api/graphics/shaders/IShader.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "impl/graphics/opengl/shaders/ShaderCompiler.h"
#include "impl/graphics/opengl/shaders/ShaderBuffers.h"

namespace Chimp::GL {
	class Shader : public IShader {
		DISABLE_COPY(Shader);
	public:
		Shader(const ShaderFilePaths& paths, const ShaderCompiler& compiler);
		~Shader() override;

		void Bind() const override;
		void Unbind() const override;

		bool IsValid() const override;

		IShaderBuffers& GetShaderBuffers() override;
		void UpdateShaderBuffer(IShaderBuffers::Index id) const override;

		void SetTextureSampler(const std::string& name, const ITexture& texture) const override;

	private:
		// Delete shaders by id and clear the vector
		void HelperDeleteShaders(std::vector<GLuint>& shaderIds);

	private:
		ShaderBuffers m_ShaderBuffers;
		GLuint m_ProgramID;
		bool m_IsValid;
	};
}