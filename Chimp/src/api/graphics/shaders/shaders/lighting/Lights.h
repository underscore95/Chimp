#pragma once

#include "utils/Maths.h"

namespace Chimp {

	struct alignas(16) PointLight {
		Vector3f Position;
		float ConstantAttenuation = 0;
		Vector3f Color;
		float LinearAttenuation = 0;
	};
	static_assert(sizeof(PointLight) % 16 == 0);

	static const int MAX_POINT_LIGHTS = 1;
	struct alignas(16) SceneLighting {
		int NumPointLights = 0;
		std::array<PointLight, MAX_POINT_LIGHTS> PointLights;
	};
	static_assert(sizeof(SceneLighting) % 16 == 0);

}