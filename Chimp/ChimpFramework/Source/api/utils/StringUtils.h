#pragma once

#include "stdafx.h"

namespace Chimp {
	void Replace(std::string_view phrase, std::string_view with, std::string& in);

	void RemoveUntilAndIncluding(std::string_view phrase, std::string& str);

	// Returns true if succeeded
	bool StringToSizeT(const std::string& str, size_t* out);

	bool StringContains(std::string_view str, std::string_view phrase);
}