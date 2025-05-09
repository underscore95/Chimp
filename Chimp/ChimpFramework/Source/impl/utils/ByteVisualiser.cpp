#include "api/utils/ByteVisualiser.h"
#include "Loggers.h"
#include "api/utils/Maths.h"

namespace Chimp {
	std::string ByteVisualiser::VisualiseBytesHex(char* start, size_t sizeInBytes)
	{
		std::ostringstream oss;
		oss << std::hex << std::setfill('0');
		for (size_t i = 0; i < sizeInBytes; ++i)
		{
			unsigned char byte = static_cast<unsigned char>(start[i]);
			oss << std::setw(2) << static_cast<int>(byte) << ' ';
		}
		return oss.str();
	}

	std::string ByteVisualiser::VisualiseBytesBinary(char* start, size_t sizeInBytes)
	{
		std::ostringstream oss;
		for (size_t i = 0; i < sizeInBytes; ++i)
		{
			unsigned char byte = static_cast<unsigned char>(start[i]);
			oss << std::bitset<8>(byte) << ' ';
		}
		return oss.str();
	}
}