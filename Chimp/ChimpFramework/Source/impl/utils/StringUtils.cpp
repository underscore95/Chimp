#include "api/utils/StringUtils.h"

namespace Chimp {
	void Replace(std::string_view phrase, std::string_view with, std::string& in)
	{
		size_t pos = 0;

		while ((pos = in.find(phrase, pos)) != std::string::npos) {
			in.replace(pos, phrase.length(), with);
			pos += with.length();
		}
	}

	void RemoveUntilAndIncluding(std::string_view phrase, std::string& str)
	{
		size_t pos = str.find(phrase);
		if (pos != std::string::npos) {
			str.erase(0, pos + phrase.length());
		}
	}

	bool StringToSizeT(const std::string& str, size_t* out)
	{
		std::istringstream iss(str);
		iss >> *out;
		return !iss.fail();
	}
}