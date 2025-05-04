#pragma once

#include "stdafx.h"

namespace Chimp {
	// Get the path relative to the file
	// e.g if you have path = "diffuse.png" and pathToFile = "res/models/monkey/monkey.obj" then will return "res/models/monkey/diffuse.png" (actually it will return an absolute path but you get the idea)
	// path must be a relative path (relative to the next argument)
	// pathToFile is a path to the file that the original path is relative to
	// returns absolute path if pathToFile is an absolute path
	// basically equivalent to (pathToFile - fileName) + path
	// if path is absolute, returns path
	// always returns an absolute path
	[[nodiscard]] std::string GetPathRelativeToFile(const std::string& path, std::string_view pathToFile);
}