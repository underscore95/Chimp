#include "api/utils/FilePath.h"

namespace Chimp {
	std::string GetPathRelativeToFile(const std::string& path, std::string_view pathToFile)
	{
		if (std::filesystem::path(path).is_absolute())
		{
			return path;
		}

		std::filesystem::path pathToFilePath(pathToFile);
		// make path absolute
		if (!pathToFilePath.is_absolute())
		{
			pathToFilePath = std::filesystem::absolute(pathToFilePath);
		}
		// remove file name from path
		pathToFilePath = pathToFilePath.remove_filename();
		// append path to file
		pathToFilePath /= path;
		return pathToFilePath.string();
	}

	std::string GetFileExtension(const std::filesystem::path& path)
	{
		return	path.has_extension() ? path.extension().string() : "";
	}

	void NormaliseSlashesInPath(std::filesystem::path& path)
	{
		std::string pathStr = path.string();
		for (char& ch : pathStr) {
			if (ch == '\\') {
				ch = '/';
			}
		}
		path = std::filesystem::path(pathStr);
	}

	std::string NormaliseFileExtension(const std::string& fileExtension)
	{
		std::string ext = fileExtension;
		// Remove leading dot if present
		if (!ext.empty() && ext[0] == '.')
			ext = ext.substr(1);

		// Convert to lower case
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		return ext;
	}
}