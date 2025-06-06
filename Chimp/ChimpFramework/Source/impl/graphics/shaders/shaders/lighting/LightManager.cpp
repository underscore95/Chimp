#include "api/graphics/shaders/shaders/lighting/LightManager.h"
#include "api/graphics/shaders/shaders/lighting/LightComponents.h"
#include "api/ecs/ECS.h"
#include "Loggers.h"

namespace Chimp {
	LightManager::LightManager(ECS& ecs) : m_ECS(ecs)
	{
	}

	SceneLighting LightManager::GenerateLighting() const
	{
		SceneLighting lighting = {};

		// Sum AmbientLights
		auto ambientLights = m_ECS.GetEntitiesWithComponents<AmbientLight>();
		for (auto& [light] : ambientLights) {
			lighting.Ambient += light.Color;
		}

		// Point Lights
		auto pointLights = m_ECS.GetEntitiesWithComponents<PointLight, TransformComponent>();
		for (auto& [light, transform] : pointLights) {
			if (lighting.NumPointLights >= MAX_POINT_LIGHTS) {
				Loggers::Rendering().Warning(std::format("Exceeded max point lights: {}", MAX_POINT_LIGHTS));
				break;
			}
			assert(!transform.IsDirty());
			light.Position = MatrixTransform({}, transform.WorldTransformMatrix);
			lighting.PointLights[lighting.NumPointLights++] = light;
		}

		// Directional Lights
		auto dirLights = m_ECS.GetEntitiesWithComponents<DirectionalLight, TransformComponent>();
		for (auto& [light, transform] : dirLights) {
			if (lighting.NumDirectionLights >= MAX_DIRECTIONAL_LIGHTS) {
				Loggers::Rendering().Warning(std::format("Exceeded max directional lights: {}", MAX_DIRECTIONAL_LIGHTS));
				break;
			}
			assert(!transform.IsDirty());
			light.Direction = MatrixTransform({ 0.0f, 0.0f, 1.0f }, To3x3(transform.WorldTransformMatrix));
			lighting.DirectionLights[lighting.NumDirectionLights++] = light;
		}

		// Spotlights
		auto spotLights = m_ECS.GetEntitiesWithComponents<Spotlight, TransformComponent>();
		for (auto& [light, transform] : spotLights) {
			if (lighting.NumSpotlights >= MAX_SPOTLIGHTS) {
				Loggers::Rendering().Warning(std::format("Exceeded max spotlights: {}", MAX_SPOTLIGHTS));
				break;
			}
			assert(!transform.IsDirty());
			light.Direction = MatrixTransform({ 0.0f, 0.0f, 1.0f }, To3x3(transform.WorldTransformMatrix));
			light.Position = MatrixTransform({}, transform.WorldTransformMatrix);
			lighting.Spotlights[lighting.NumSpotlights++] = light;
		}

		return lighting;
	}

}