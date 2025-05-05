#pragma once

#include "stdafx.h"
#include "api/ecs/EntityId.h"

namespace Chimp {
	class ECS;
	class TransformComponent;
	class HierarchyComponent;
	typedef ConstOptionalReference<TransformComponent> TransformSnapshot;

	class TransformManager {
		friend class ECS;

		TransformManager(ECS& ecs);
	public:
		TransformSnapshot GetTransformSnapshot(EntityId entity);

		Vector3f GetGlobalPosition(EntityId entity);
		void SetGlobalPosition(EntityId entity, Vector3f position);

		Vector3f GetLocalPosition(EntityId entity);
		void SetLocalPosition(EntityId entity, Vector3f position);

		Quaternion GetLocalRotation(EntityId entity);
		void SetLocalRotation(EntityId entity, Quaternion rotation);
		void SetLocalRotation(EntityId entity, Vector3f eulerRotationDegrees);

		Vector3f GetLocalScale(EntityId entity);
		void SetLocalScale(EntityId entity, Vector3f scale);

		void UpdateAllMatrices();

	private:
		[[nodiscard]] OptionalReference<TransformComponent> GetMutableTransform(EntityId entity);

		void MarkDirty(EntityId entity, OptionalReference<TransformComponent> transform);

	private:
		ECS& m_ECS;
	};
}