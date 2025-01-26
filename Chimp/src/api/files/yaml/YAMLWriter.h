#pragma once

#include "stdafx.h"
#include "api/files/yaml/YAMLBlock.h"

namespace Chimp {
	class YAMLWriter {
		YAMLWriter() = delete;
	public:
		// Writes the YAMLBlock to a vector of strings
		// out - vector to write to
		static void Write(const YAMLBlock& root, std::vector<std::string>& out);

		// Writes the YAMLBlock to file
		// path - path to write to
		static void Write(const YAMLBlock& root, const std::filesystem::path& path);

		// Writes the YAMLBlock
		// writeFunc - called with each line of the YAML
		// indents - number of indents to start with
		static void Write(const YAMLBlock& root, std::function<void(std::string_view)> writeFunc, int numIndents = 0);
	};
}