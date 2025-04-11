#pragma once

#include "api/utils/Maths.h"
#include "Loggers.h"

namespace Chimp {
	struct alignas(16) PointLightMatrices {
		Matrix Projection;
		std::array<Matrix, 6> Views;
	};
	static_assert(sizeof(PointLightMatrices) % 16 == 0);

	// POINT LIGHT
	static const int MAX_POINT_LIGHTS = 1;
	struct alignas(16) PointLight {
		Vector3f Position;
		float Padding;

		Vector3f Color;
		float Padding2;

		Vector3f Attenuation;
		float Padding3;

		float FarPlane = 20.0f; 
		int CubeRootNumShadowSamples = 4;
		float ShadowBias = 0.15f;
		float ShadowMaxSampleDistance = 0.15f;

		PointLightMatrices CalculateMatrices(float aspectRatio = 1, float zNear = 1.0f) const {
			PointLightMatrices matrices = {};
			matrices.Projection = CreatePerspectiveProjectionMatrix(90, aspectRatio, zNear, FarPlane);

			matrices.Views = {
				CreateViewMatrix(Position, Position + Vector3f(1.0f,  0.0f,  0.0f), Vector3f(0.0f, -1.0f,  0.0f)),
				CreateViewMatrix(Position, Position + Vector3f(-1.0f,  0.0f,  0.0f), Vector3f(0.0f, -1.0f,  0.0f)),
				CreateViewMatrix(Position, Position + Vector3f(0.0f,  1.0f,  0.0f), Vector3f(0.0f,  0.0f,  1.0f)),
				CreateViewMatrix(Position, Position + Vector3f(0.0f, -1.0f,  0.0f), Vector3f(0.0f,  0.0f, -1.0f)),
				CreateViewMatrix(Position, Position + Vector3f(0.0f,  0.0f,  1.0f), Vector3f(0.0f, -1.0f,  0.0f)),
				CreateViewMatrix(Position, Position + Vector3f(0.0f,  0.0f, -1.0f), Vector3f(0.0f, -1.0f,  0.0f))
			};

			return matrices;
		}
	};
	static_assert(sizeof(PointLight) % 16 == 0);

	// DIRECTIONAL LIGHT
	static const int MAX_DIRECTIONAL_LIGHTS = 1;
	struct alignas(16) DirectionalLight {
		Vector3f Direction;
		float Padding;

		Vector3f Color;
		float Padding2;

		CameraMatrices CalculateMatrices(Rect bounds, float zNear = 1.0f, float zFar = 20.0f, Vector3f up = { 0,1,0 }) const {
			CameraMatrices matrices;
			matrices.SetProjectionMatrix(CreateOrthographicProjectionMatrix(bounds.GetLeft(), bounds.GetRight(), bounds.GetBottom(), bounds.GetTop(), zNear, zFar));
			assert(IsNormalised(Direction));

			MakeUpVectorValid(&up, Direction);

			Vector3f cameraPos = Direction * -10.0f;
			matrices.SetViewMatrix(CreateViewMatrix(cameraPos, { 0,0,0 }, up));
			return matrices;
		}
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
		float Padding43;

		Vector3f Attenuation;
		float CutoffAngle; // Should be Cos(angle)

		CameraMatrices CalculateMatrices(float cutoffAngleDegrees, float aspectRatio = 1, float zNear = 5.0f, float zFar = 50.0f, Vector3f up = { 0,1,0 }) const {
			CameraMatrices matrices;
			assert(FloatEqual(Cos(cutoffAngleDegrees), CutoffAngle));
			matrices.SetProjectionMatrix(CreatePerspectiveProjectionMatrix(cutoffAngleDegrees * 2, aspectRatio, zNear, zFar));
			assert(IsNormalised(Direction));

			MakeUpVectorValid(&up, Direction);

			matrices.SetViewMatrix(CreateViewMatrix(Position, Position + Direction, up));
			return matrices;
		}
	};
	static_assert(sizeof(Spotlight) % 16 == 0);

	// LIGHT MATRICES
	struct alignas(16) LightMatrices {
		std::array<Matrix, MAX_SPOTLIGHTS> Spotlights;
		std::array<Matrix, MAX_DIRECTIONAL_LIGHTS> DirectionalLights;
		int NumSpotlights;
		int NumDirectionalLights;
		int Padding2;
		int Padding3;
	};
	static_assert(sizeof(LightMatrices) % 16 == 0);

	// LIGHTING
	struct alignas(16) SceneLighting {
		Vector3f Ambient = { 0,0,0 };
		float Padding = 0;
		std::array<PointLight, MAX_POINT_LIGHTS> PointLights;
		std::array<DirectionalLight, MAX_DIRECTIONAL_LIGHTS> DirectionLights;
		std::array<Spotlight, MAX_SPOTLIGHTS> Spotlights;
		int NumPointLights = 0;
		int NumDirectionLights = 0;
		int NumSpotlights = 0;
		int IsDepthPass = 0;
	};
	static_assert(sizeof(SceneLighting) % 16 == 0);

}