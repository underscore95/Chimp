#pragma once

#include "utils/Maths.h"

namespace Chimp {

	// POINT LIGHT
	static const int MAX_POINT_LIGHTS = 1;
	struct alignas(16) PointLight {
		Vector3f Position;
		float Padding;

		Vector3f Color;
		float Padding2;

		Vector3f Attenuation;
		float Padding3;
	};
	static_assert(sizeof(PointLight) % 16 == 0);

	// DIRECTIONAL LIGHT
	static const int MAX_DIRECTIONAL_LIGHTS = 1;
	struct alignas(16) DirectionalLight {
		Vector3f Direction;
		float Padding;

		Vector3f Color;
		float Padding2;
	};	
	static_assert(sizeof(DirectionalLight) % 16 == 0);

	// SPOTLIGHT
	static const int MAX_SPOTLIGHTS = 1;
	struct alignas(16) Spotlight {
		Vector3f Direction;
		float Padding;

		Vector3f Position;
		float Padding2;

		Vector3f Color;
		float Padding4;

		Vector3f Attenuation;
		float CutoffAngle; // Should be Cos(angle)
	};	
	static_assert(sizeof(Spotlight) % 16 == 0);

	// LIGHTING
	struct alignas(16) SceneLighting {
		Vector3f Ambient = { 0,0,0 };
		float Padding = 0;
		std::array<PointLight, MAX_POINT_LIGHTS> PointLights;
		std::array<DirectionalLight, MAX_DIRECTIONAL_LIGHTS> DirectionLights;
		std::array<Spotlight, MAX_SPOTLIGHTS> Spotlights;
		int NumPointLights = 0;
		int NumDirectionLights = 0;
		int NumSpotlightsLights = 0;
		float Padding2 = 0;
	};
	static_assert(sizeof(SceneLighting) % 16 == 0);

}