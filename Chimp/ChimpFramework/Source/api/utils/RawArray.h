#pragma once

#include "stdafx.h"
#include "api/utils/preprocessor/CopyAndMove.h"

namespace Chimp {
	struct RawArray {
		DISABLE_COPY(RawArray);
		RawArray() = default;
		RawArray(RawArray&& toMove) noexcept;
		RawArray& operator=(RawArray&& toMove) noexcept;
		~RawArray();

		size_t Size; // Size in bytes of the whole array
		size_t NumberElements; // Number of elements in the array
		void* Data; // Pointer to the data, this is owned by the RawArray
	};
}