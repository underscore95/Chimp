#include "api/graphics/shaders/IShader.h"
#include "api/graphics/buffers/IBuffer.h"

namespace Chimp {
	void IShader::SetShaderBufferSubData(
		IShaderBuffers::Index id,
		const void* data,
		const size_t size,
		const size_t offset)
	{
		auto& buffer = GetShaderBuffers().GetBuffer(id);
		buffer.Buffer->SetSubData(data, size, offset);
		UpdateShaderBuffer(id);
	}

	void IShader::SetShaderBufferSubData(IShaderBuffers::Index id, const RawArray& rawArray, const size_t offset)
	{
		SetShaderBufferSubData(id, rawArray.Data, rawArray.Size, offset);
	}
}