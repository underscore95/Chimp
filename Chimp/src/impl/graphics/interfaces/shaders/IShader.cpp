#include "api/graphics/shaders/IShader.h"
#include "api/graphics/buffers/IBuffer.h"
#include "api/graphics/textures/ITexture.h"

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

	void IShader::SetTextureSampler(const std::string& name, const ITexture& texture) const
	{
		texture.Bind(); // Exception here? probably you loaded a texture that didn't exist, check the console!
		SetTextureSampler(name, texture.GetSlot());
	}
}