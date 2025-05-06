#include "api/graphics/shaders/IShaderBuffers.h"
#include "api/graphics/shaders/IShader.h"

namespace Chimp {
	IShaderBuffers::Index IShaderBuffers::AddBuffer(ShaderBuffer buffer)
	{
		assert(m_Buffers.size() < GetMaximumBufferCount());
		assert(buffer.Buffer != nullptr);
		buffer.Index = m_Buffers.size();
		m_Buffers.push_back(buffer);
		m_Shader.UpdateShaderBuffer(buffer.Index);
		return buffer.Index;
	}

	// If removing is implemented, make sure it doesn't break ids of other buffers
	// maybe setting to nullptr instead of removing from the vector?
	// and do some fancy stuff to reuse nullptr values in AddBuffer? (this would require changing the assert too)

	const ShaderBuffer& IShaderBuffers::GetBuffer(Index id) const
	{
		auto& buffer = m_Buffers[id];
		assert(buffer.Buffer != nullptr);
		return buffer;
	}

	ShaderBuffer& IShaderBuffers::GetBuffer(Index id)
	{
		auto& buffer = m_Buffers[id];
		assert(buffer.Buffer != nullptr);
		return buffer;
	}
}