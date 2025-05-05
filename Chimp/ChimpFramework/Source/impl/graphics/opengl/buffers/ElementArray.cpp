#include "ElementArray.h"

namespace Chimp::GL {
	ElementArray::ElementArray(std::shared_ptr<IBuffer> vertexBuffer,
		std::unique_ptr<IBuffer> indexBuffer,
		GraphicsType indexType,
		std::shared_ptr<IElementArrayLayout> layout) :
		IElementArray(std::move(vertexBuffer), std::move(indexBuffer), indexType, std::move(layout))
	{
		glGenVertexArrays(1, &m_Id);
		Bind();

		GetVertexBuffer().Bind();
		GetIndexBuffer().Bind();
		GetElementLayout().Bind();

		Unbind();
	}

	void ElementArray::Bind() const
	{
		glBindVertexArray(m_Id);
	}

	void ElementArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}