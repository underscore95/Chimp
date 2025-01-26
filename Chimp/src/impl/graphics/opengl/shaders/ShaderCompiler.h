#pragma once

#include "api/graphics/shaders/IShaderCompiler.h"

namespace Chimp::GL {
	class ShaderCompiler : public IShaderCompiler<GLuint> {
	public:
		ShaderCompiler() = default;
		~ShaderCompiler() = default;

		Result Compile(const ShaderType type, const std::string& source) const override;
	};
}