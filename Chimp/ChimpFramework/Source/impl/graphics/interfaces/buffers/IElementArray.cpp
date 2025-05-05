#include "api/graphics/buffers/IElementArray.h"

namespace Chimp {
	IElementArray::IElementArray(std::shared_ptr<IBuffer> vertexBuffer,
		std::unique_ptr<IBuffer> indexBuffer,
		GraphicsType indexType,
		std::shared_ptr<IElementArrayLayout> layout
	) :
		m_VertexBuffer(std::move(vertexBuffer)),
		m_IndexBuffer(std::move(indexBuffer)),
		m_IndexType(indexType),
		m_Layout(std::move(layout))
	{
	}

	IBuffer& IElementArray::GetVertexBuffer()
	{
		return *m_VertexBuffer;
	}

	IBuffer& IElementArray::GetIndexBuffer()
	{
		return *m_IndexBuffer;
	}

	const IElementArrayLayout& IElementArray::GetElementLayout() const
	{
		return *m_Layout;
	}

	void IElementArray::SetLayout(std::shared_ptr<IElementArrayLayout> layout)
	{
		m_Layout = std::move(layout);
	}

	unsigned int IElementArray::GetIndexCount() const
	{
		return static_cast<unsigned int>(m_IndexBuffer->GetNumberElements());
	}

	GraphicsType IElementArray::GetIndexType() const
	{
		return m_IndexType;
	}

	PrimitiveType IElementArray::GetPrimitiveType() const
	{
		return m_Layout->GetPrimitiveType();
	}
}