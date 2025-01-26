#pragma once

namespace Chimp {
	struct YAMLBlock {
		std::unordered_map<std::string, bool> Bools;
		std::unordered_map<std::string, int> Ints;
		std::unordered_map<std::string, float> Floats;
		std::unordered_map<std::string, std::string> Strings;
		std::unordered_map<std::string, std::vector<std::string>> StringArrays;
		std::unordered_map<std::string, std::vector<int>> IntArrays;
		std::unordered_map<std::string, std::vector<float>> FloatArrays;
		std::unordered_map<std::string, std::vector<bool>> BoolArrays;
		std::unordered_map<std::string, YAMLBlock> Blocks;
	};
}