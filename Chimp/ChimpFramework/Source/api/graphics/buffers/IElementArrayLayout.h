#pragma once

#include "stdafx.h"
#include "api/graphics/GraphicsType.h"
#include "api/graphics/PrimitiveType.h"

namespace Chimp {
	// Represents a single component of a vertex
	// this could be position, uv coordinates, etc
	// multiple of these will make up a vertex
	struct ElementComponentLayout {
		GraphicsType Type; // type of elements
		unsigned int Count; // number of elements
		bool Normalized; // normalize the data (map unsigned integers to floats in range 0 to 1) (ints mapped to -1 to 1)
	};

	// Represents the layout of the vertices in an element array
	class IElementArrayLayout {
	protected:
		IElementArrayLayout(const PrimitiveType primitiveType, 
			const std::vector<ElementComponentLayout>& layouts);

	public:
		virtual ~IElementArrayLayout() = default;

		// Bind the layout
		virtual void Bind() const = 0;

		// Get the primitive type
		[[nodiscard]] virtual const PrimitiveType GetPrimitiveType() const;

		// Get the stride (size of a single vertex)
		[[nodiscard]] virtual const unsigned int GetStride() const;

	private:
		static unsigned int CalculateStride(const std::vector<ElementComponentLayout>& layouts);

	protected:
		const PrimitiveType m_PrimitiveType;
		const std::vector<ElementComponentLayout> m_Layouts;
		const unsigned int m_Stride;
	};
}