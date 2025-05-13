#include "api/utils/FileReader.h"

namespace Chimp {
	std::string FileReader::Read(const std::string& path) {
		return Read(std::filesystem::path(path));
	}

	std::string FileReader::Read(const std::filesystem::path& path) {
		std::stringstream ss;
		std::ifstream file(path);

		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << path << std::endl;
			return ss.str();
		}

		ss << file.rdbuf();
		file.close();
		return ss.str();
	}
}