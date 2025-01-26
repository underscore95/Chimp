#pragma once

#include "stdafx.h"
#include "api/utils/preprocessor/CopyAndMove.h"

namespace Chimp {
	class IShader;
	class IBuffer;

	struct ShaderBuffer {
		// Represents a buffer bound to a shader, data will be sent into a uniform block (or similar if not opengl)
		// buffer - the buffer to bind
		// name - the name of the buffer in the shader
		ShaderBuffer(std::shared_ptr<IBuffer> buffer, const std::string& name)
			: Buffer(buffer), Name(name), Index(0) {}

		std::shared_ptr<IBuffer> Buffer;
		std::string Name;
		size_t Index; // Don't worry about this, it will be set by the shader
	};

	// Represents the buffers bound to a shader
	// This class should only exist as a field of an IShader implementation
	class IShaderBuffers {
		DISABLE_COPY(IShaderBuffers);
		friend class IShader;
	protected:
		IShaderBuffers(IShader& shader) : m_Shader(shader) {}

	public:
		// A shader-specific identifier for a buffer
		typedef size_t Index;

		// Add a bufffer to the shader
		// Returns a shader-specific identifier for the buffer
		// This identifier is required for actions such as updating the buffer
		Index AddBuffer(ShaderBuffer buffer);

		// Get a buffer by its identifier
		[[nodiscard]] const ShaderBuffer& GetBuffer(Index id) const;
		[[nodiscard]] ShaderBuffer& GetBuffer(Index id);
	protected:
		// Get the maximum number of buffers that can be bound to a shader
		[[nodiscard]] virtual Index GetMaximumBufferCount() const = 0;

	private:
		IShader& m_Shader;
		std::vector<ShaderBuffer> m_Buffers;
	};

	using ShaderBufferId = IShaderBuffers::Index;
}