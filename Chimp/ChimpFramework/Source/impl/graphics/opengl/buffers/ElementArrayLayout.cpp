#include "ElementArrayLayout.h"
#include "impl/graphics/opengl/Translations.h"

namespace Chimp::GL {
	ElementArrayLayout::ElementArrayLayout(
		const PrimitiveType primitiveType,
		const std::vector<ElementComponentLayout>& layouts) :
		IElementArrayLayout(primitiveType, layouts)
	{
	}

	void ElementArrayLayout::Bind() const
	{
		unsigned int offset = 0;
		for (unsigned int index = 0; index < m_Layouts.size(); index++) {
			const auto& layout = m_Layouts[index];
			glEnableVertexAttribArray(index); // todo: what happens if we bind one with 3 layouts and another with 2? won't the 3rd layout still be enabled?
			glVertexAttribPointer(index,
				layout.Count,
				TypeTranslation(layout.Type),
				layout.Normalized,
				m_Stride,
				(const void*)offset);
			offset += layout.Count * layout.Type.Size;
		}
	}
}