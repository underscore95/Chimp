#include "api/utils/RawArray.h"

namespace Chimp {
	RawArray::RawArray(RawArray&& toMove) noexcept {
		Size = toMove.Size;
		NumberElements = toMove.NumberElements;
		Data = toMove.Data;
		toMove.Data = nullptr;
	}

	RawArray& RawArray::operator=(RawArray&& toMove) noexcept {
		if (Data) {
			delete[] Data;
		}

		Size = toMove.Size;
		NumberElements = toMove.NumberElements;
		Data = toMove.Data;
		toMove.Data = nullptr;
		return *this;
	}

	RawArray::~RawArray()
	{
		if (Data) {
			delete[] Data;
		}
	}
}