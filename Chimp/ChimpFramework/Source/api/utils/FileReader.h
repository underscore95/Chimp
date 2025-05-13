#pragma once

#include "stdafx.h"

namespace Chimp {
	class FileReader {
		FileReader() = delete;
	public:
		// Read contents of a file into a string
		// if the reading fails, an empty string is returned
		static std::string Read(const std::string& path);
		static std::string Read(const std::filesystem::path& path);
	};
}