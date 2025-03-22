#pragma once

#include "utils/Maths.h"

namespace Chimp {

	struct PointLight {
		Vector3f Position;
		float ConstantAttenuation;
		Vector3f Colour;
		float LinearAttenuation;
	};

	static const int MAX_POINT_LIGHTS = 1;
	struct SceneLighting {
		int NumPointLights = 0;
		std::array<PointLight, MAX_POINT_LIGHTS> PointLights;
	};
}