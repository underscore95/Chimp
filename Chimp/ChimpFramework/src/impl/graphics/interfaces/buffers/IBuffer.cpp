#include "api/graphics/buffers/IBuffer.h"

namespace Chimp {
	void IBuffer::SetData(const RawArray& data)
	{
		Bind();
		SetDataBindless(data);
	}

	void IBuffer::SetSubData(const void* data, const size_t size, size_t offset)
	{
		Bind();
		SetSubDataBindless(data, size, offset);
	}

	void IBuffer::SetSubData(const RawArray& data, size_t offset)
	{
		Bind();
		SetSubDataBindless(data, offset);
	}

	void IBuffer::SetSubDataBindless(const RawArray& data, size_t offset)
	{
		SetSubDataBindless(data.Data, data.Size, offset);
	}

	size_t IBuffer::GetSize() const
	{
		return m_Size;
	}

	size_t IBuffer::GetNumberElements() const
	{
		return m_NumberElements;
	}

	void IBuffer::Resize(size_t size, size_t numberElements)
	{
		m_Size = size;
		m_NumberElements = numberElements;
	}
}