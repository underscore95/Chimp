#pragma once

#include "stdafx.h"
#include "api/ecs/EntityId.h"

namespace Chimp {
	class ECS;
	class TransformComponent;
	typedef ConstOptionalReference<TransformComponent> TemporaryTransform;

	class TransformManager {
		friend class ECS;

		TransformManager(ECS& ecs);
	public:
		TemporaryTransform GetTransform(EntityId entity);

	private:
		[[nodiscard]] OptionalReference<TransformComponent> GetMutableTransform(EntityId entity);

	private:
		ECS& m_ECS;
	};
}