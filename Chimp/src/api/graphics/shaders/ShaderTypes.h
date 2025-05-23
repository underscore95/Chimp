#pragma once

#include "stdafx.h"
#include "api/utils/FileReader.h"
#include "api/utils/HashCombine.h"

namespace Chimp {
	enum class ShaderType {
		VERTEX,
		GEOMETRY,
		FRAGMENT
	};

	struct ShaderFilePaths {
		std::string Vertex;
		std::string Geometry;
		std::string Fragment;

		std::unordered_map<ShaderType, std::string> AsMap() const {
			return {
				{ ShaderType::VERTEX, Vertex },
				{ ShaderType::GEOMETRY, Geometry },
				{ ShaderType::FRAGMENT, Fragment }
			};
		}

		bool operator==(const ShaderFilePaths& other) const {
			return Vertex == other.Vertex && Fragment == other.Fragment && Geometry == other.Geometry;
		}
	};
}

MAKE_HASHABLE(Chimp::ShaderFilePaths, t.Vertex, t.Geometry, t.Fragment);