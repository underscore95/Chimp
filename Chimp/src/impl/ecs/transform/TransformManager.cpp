#include "api/ecs/transform/TransformManager.h"
#include "api/ecs/ECS.h"
#include "api/ecs/Components.h"

namespace Chimp {
	TransformManager::TransformManager(ECS& ecs)
		: m_ECS(ecs) {

	}

	TemporaryTransform TransformManager::GetTransform(EntityId entity)
	{
		return GetMutableTransform(entity);
	}

	OptionalReference<TransformComponent> TransformManager::GetMutableTransform(EntityId entity)
	{
		auto transform = m_ECS.GetMutableComponent<TransformComponent>(entity);

		if (!transform) {
			// Return parent transform
			EntityId parent;
			return m_ECS.TryGetParent(entity, parent) ? GetMutableTransform(parent) : nullptr;
		}

		if (!transform->IsDirty()) return transform;

		// Update the local matrix
		transform->LocalTransformMatrix = CreateTransformMatrix(transform->LocalTranslation, transform->LocalRotation, transform->LocalScale);

		// Get the parent matrix
		EntityId parent;
		if (m_ECS.TryGetParent(entity, parent)) {

			// Calculate the world matrix
			auto parentTransform = GetMutableTransform(parent);
			transform->WorldTransformMatrix = parentTransform ? (parentTransform->WorldTransformMatrix * transform->LocalTransformMatrix) : transform->LocalTransformMatrix; }
		else {
			transform->WorldTransformMatrix = transform->LocalTransformMatrix;
		}

		transform->m_IsDirty = false;
		return transform;
	}
}