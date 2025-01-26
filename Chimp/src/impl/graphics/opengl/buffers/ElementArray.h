#pragma once

#include "api/graphics/buffers/IElementArray.h"

namespace Chimp::GL {
	class ElementArray : public IElementArray {
	public:
		ElementArray(
			std::shared_ptr<IBuffer> vertexBuffer,
			std::unique_ptr<IBuffer> indexBuffer,
			GraphicsType indexType,
			std::shared_ptr<IElementArrayLayout> layout
		);
		~ElementArray() = default;

		void Bind() const override;

		void Unbind() const override;

	private:
		GLuint m_Id;
	};
}