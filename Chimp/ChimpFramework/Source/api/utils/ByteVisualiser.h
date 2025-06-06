#pragma once

#include "stdafx.h"
#include "api/utils/AnyReference.h"

namespace Chimp {
	class ByteVisualiser {
		ByteVisualiser() = delete;

	public:
		static std::string VisualiseBytesHex(char* start, size_t sizeInBytes);
		static std::string VisualiseBytesBinary(char* start, size_t sizeInBytes);
	};
}