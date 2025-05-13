#pragma once

#include "stdafx.h"

namespace Chimp {
	void Replace(std::string_view phrase, std::string_view with, std::string& in);
	void RemoveUntilAndIncluding(std::string_view phrase, std::string& str);
}