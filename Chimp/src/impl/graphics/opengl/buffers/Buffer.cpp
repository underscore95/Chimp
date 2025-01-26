#include "Buffer.h"
#include "impl/graphics/opengl/Translations.h"

namespace Chimp::GL {
	Buffer::Buffer(
		const Usage& usage,
		const BindTarget target
	) :
		m_Usage(usage),
		m_GLUsageHint(UsageTranslation(usage)),
		m_BindTarget(BindTargetTranslation(target))
	{
		glGenBuffers(1, &m_Id);
	}

	Buffer::~Buffer() {
		glDeleteBuffers(1, &m_Id);
	}

	void Buffer::Bind() {
		glBindBuffer(m_BindTarget, m_Id);
	}

	void Buffer::SetBindTarget(const BindTarget target)
	{
		m_BindTarget = BindTargetTranslation(target);
	}

	void Buffer::SetDataBindless(const RawArray& data)
	{
		assert(data.Data != nullptr);
		Resize(data.Size, data.NumberElements);
		glBufferData(m_BindTarget, data.Size, data.Data, m_GLUsageHint);
	}

	void Buffer::SetSubDataBindless(const void* data, const size_t size, const size_t offset)
	{
		assert(offset + size <= GetSize());
		assert(data != nullptr);
		glBufferSubData(m_BindTarget, offset, size, data);
	}

	void Buffer::BindBufferBase(const size_t index) const
	{
		glBindBufferBase(m_BindTarget, index, m_Id);
	}


}