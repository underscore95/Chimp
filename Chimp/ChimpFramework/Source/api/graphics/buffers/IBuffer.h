#pragma once

#include "stdafx.h"
#include "api/utils/RawArray.h"
#include "Usage.h"
#include "api/graphics/shaders/IShaderBuffers.h"

namespace Chimp {
	class IBuffer {
		DISABLE_COPY(IBuffer);
	protected:
		IBuffer() = default;

	public:
		~IBuffer() = default;

	public:
		// Bind the buffer
		virtual void Bind() = 0;

		// Change the bind target, this will not take effect until Bind() is called
		virtual void SetBindTarget(BindTarget target) = 0;

		// Set the data of the buffer, this will call Bind()
		// dataTypes is the type of data in the RawArray
		// data is the data to set, this can be deleted after this function is called
		void SetData(const RawArray& data);

		// Set the data of the buffer, this will not call Bind()
		virtual void SetDataBindless(const RawArray& data) = 0;

		// Set the sub data of the buffer, this will call Bind()
		void SetSubData(const void* data, const size_t size, size_t offset);
		void SetSubData(const RawArray& data, size_t offset);

		// Set the sub data of the buffer, this will not call Bind()
		virtual void SetSubDataBindless(const void* data, const size_t size, size_t offset) = 0;
		void SetSubDataBindless(const RawArray& data, size_t offset);

		// Get the size (number of bytes) of the buffer
		[[nodiscard]] size_t GetSize() const;

		// Get the number of elements in the buffer
		[[nodiscard]] size_t GetNumberElements() const;

		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBufferBase.xhtml
		virtual void BindBufferBase(const size_t index) const = 0;

	protected:
		void Resize(size_t size, size_t numberElements);

	private:
		size_t m_NumberElements{ 0 };
		size_t m_Size{ 0 };
	};
}