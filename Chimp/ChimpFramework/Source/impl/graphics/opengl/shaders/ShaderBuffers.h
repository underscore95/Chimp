#pragma once

#include "api/graphics/shaders/IShaderBuffers.h"

namespace Chimp::GL {
	class ShaderBuffers : public IShaderBuffers
	{
	public:
		ShaderBuffers(IShader& shader) : IShaderBuffers(shader) {}
		~ShaderBuffers() = default;

		[[nodiscard]] Index GetMaximumBufferCount() const override {
			GLint maxBuffers;
			glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBuffers);
			return static_cast<size_t>(maxBuffers);
		}
	};
}