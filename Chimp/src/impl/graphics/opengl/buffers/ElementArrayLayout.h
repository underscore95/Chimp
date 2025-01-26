#pragma once

#include "api/graphics/buffers/IElementArrayLayout.h"

namespace Chimp::GL {
	class ElementArrayLayout : public IElementArrayLayout {
	public:
		ElementArrayLayout(
			const PrimitiveType primitiveType,
			const std::vector<ElementComponentLayout>& layouts
		);
		~ElementArrayLayout() = default;

		void Bind() const override;
	};
}