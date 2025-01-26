#pragma once

#include "stdafx.h"
#include "IShaderBuffers.h"
#include "api/utils/RawArray.h"

namespace Chimp {
	class ITexture;

	// Represents all the shaders in a pipeline
	// Vertex, fragment, etc
	class IShader {
		DISABLE_COPY(IShader);

	protected:
		IShader() = default;

	public:
		virtual ~IShader() = default;

		// Bind the shader
		virtual void Bind() const = 0;

		// Unbind the shader
		virtual void Unbind() const = 0;

		// Check if the shader is valid (all shaders compiled successfully)
		virtual bool IsValid() const = 0;

		// Get the shader buffers
		virtual [[nodiscard]] IShaderBuffers& GetShaderBuffers() = 0;

		// Bind the buffers to the shader
		// this must be called whenever the data (or subdata) in the IBuffer has changed
		virtual void UpdateShaderBuffer(IShaderBuffers::Index id) const = 0;

		// Set the subdata inside and then update a shader buffer
		// id - the index of the buffer
		// data - the data to set, this can be deleted after the function returns
		// size - the size of the data in bytes
		// offset - the offset to start writing the data (Default 0)
		virtual void SetShaderBufferSubData(
			IShaderBuffers::Index id, 
			const void* data,
			const size_t size, 
			const size_t offset = 0);
		// rawArray - the data to set, this can be deleted after the function returns
		virtual void SetShaderBufferSubData(
			IShaderBuffers::Index id,
			const RawArray& rawArray,
			const size_t offset = 0);

		// Set a texture sampler in the shader
		// This function will bind both the shader and the texture.
		// name - the name of the sampler uniform in the shader
		// texture - the texture to send
		virtual void SetTextureSampler(const std::string& name, const ITexture& texture) const = 0;
	};
}