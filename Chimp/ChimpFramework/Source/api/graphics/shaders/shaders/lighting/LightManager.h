#pragma once

#include "stdafx.h"

namespace Chimp {
	class ECS;
	class SceneLighting;

	class LightManager {
		friend class ECS;
	private:
		LightManager(ECS& ecs);

	public:
		SceneLighting GenerateLighting() const;

	private:
		ECS& m_ECS;
	};
}