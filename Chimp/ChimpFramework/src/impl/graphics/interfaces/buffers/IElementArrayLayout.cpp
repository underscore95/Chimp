#include "api/graphics/buffers/IElementArrayLayout.h"

namespace Chimp {
	IElementArrayLayout::IElementArrayLayout(
		const PrimitiveType primitiveType,
		const std::vector<ElementComponentLayout>& layouts) :
		m_PrimitiveType(primitiveType),
		m_Layouts(layouts), 
		m_Stride(CalculateStride(layouts))
	{
	}

	const PrimitiveType IElementArrayLayout::GetPrimitiveType() const
	{
		return m_PrimitiveType;
	}

	const unsigned int IElementArrayLayout::GetStride() const
	{
		return m_Stride;
	}

	unsigned int Chimp::IElementArrayLayout::CalculateStride(const std::vector<ElementComponentLayout>& layouts)
	{
		unsigned int stride = 0;
		for (const auto& layout : layouts) {
			stride += layout.Count * layout.Type.Size;
		}
		return stride;
	}
}