#pragma once

#include "api/graphics/buffers/IBuffer.h"

namespace Chimp::GL {
	class Buffer : public IBuffer {
	public:
		Buffer(
			const Usage& usage, 
			const BindTarget target);
		~Buffer();

		void Bind() override;

		void SetBindTarget(const BindTarget target) override;

		void SetDataBindless(const RawArray& data) override;

		void SetSubDataBindless(const void* data, const size_t size, const size_t offset) override;

		void BindBufferBase(const size_t index) const override;

	private:
		GLuint m_Id{ 0 };
		const Usage m_Usage;
		const GLenum m_GLUsageHint;
		GLenum m_BindTarget;
	};
}