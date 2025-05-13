#include "api/ecs/transform/TransformManager.h"
#include "api/ecs/ECS.h"
#include "api/ecs/Components.h"

namespace Chimp {
	TransformManager::TransformManager(ECS& ecs)
		: m_ECS(ecs) {

	}

	TransformSnapshot TransformManager::GetTransformSnapshot(EntityId entity)
	{
		return GetMutableTransform(entity);
	}

	Vector3f TransformManager::GetGlobalPosition(EntityId entity)
	{
		auto transform = GetTransformSnapshot(entity);
		assert(transform);
		return MatrixTransform({}, transform->WorldTransformMatrix);
	}

	void TransformManager::SetGlobalPosition(EntityId entity, Vector3f position)
	{
		auto transform = GetMutableTransform(entity);
		assert(transform);
		Vector3f currentPosition = MatrixTransform({}, transform->WorldTransformMatrix);
		Vector3f deltaPosition = position - currentPosition;
		transform->LocalTranslation += deltaPosition;
		MarkDirty(entity, transform);
	}

	Vector3f TransformManager::GetLocalPosition(EntityId entity)
	{
		auto transform = GetMutableTransform(entity);
		assert(transform);
		return transform->LocalTranslation;
	}

	void TransformManager::SetLocalPosition(EntityId entity, Vector3f position)
	{
		auto transform = GetMutableTransform(entity);
		assert(transform);
		transform->LocalTranslation = position;
		MarkDirty(entity, transform);
	}

	Quaternion TransformManager::GetLocalRotation(EntityId entity)
	{
		auto transform = GetMutableTransform(entity);
		assert(transform);
		return transform->LocalRotation;
	}

	void TransformManager::SetLocalRotation(EntityId entity, Quaternion rotation)
	{
		auto transform = GetMutableTransform(entity);
		assert(transform);
		transform->LocalRotation = rotation;
		MarkDirty(entity, transform);
	}

	void TransformManager::SetLocalRotation(EntityId entity, Vector3f eulerRotationDegrees)
	{
		SetLocalRotation(entity, ToQuatRotation(eulerRotationDegrees));
	}

	Vector3f TransformManager::GetLocalScale(EntityId entity)
	{
		auto transform = GetMutableTransform(entity);
		assert(transform);
		return transform->LocalScale;
	}

	void TransformManager::SetLocalScale(EntityId entity, Vector3f scale)
	{
		auto transform = GetMutableTransform(entity);
		assert(transform);
		transform->LocalScale = scale;
		MarkDirty(entity, transform);
	}

	void TransformManager::SetTranslationRotationScale(EntityId entity, Vector3f translation, Quaternion rotation, Vector3f scale)
	{
		auto transform = GetMutableTransform(entity);
		assert(transform);
		transform->LocalTranslation = translation;
		transform->LocalRotation = rotation;
		transform->LocalScale = scale;
		MarkDirty(entity, transform);
	}

	void TransformManager::UpdateAllMatrices()
	{
		auto view = m_ECS.GetEntitiesWithComponents <EntityIdComponent, TransformComponent>();
		for (auto& [entityId, transform] : view) {
			auto _ = GetMutableTransform(entityId.Id);
		}
	}

	OptionalReference<TransformComponent> TransformManager::GetMutableTransform(EntityId entity)
	{
		auto transform = m_ECS.GetMutableComponent<TransformComponent>(entity);

		if (!transform) {
			// Return parent transform
			EntityId parent;
			return m_ECS.GetHierarchy().TryGetParent(entity, parent) ? GetMutableTransform(parent) : nullptr;
		}

		if (!transform->IsDirty()) return transform;

		// Update the local matrix
		transform->LocalTransformMatrix = CreateTransformMatrix(transform->LocalTranslation, transform->LocalRotation, transform->LocalScale);

		// Get the parent matrix
		EntityId parent;
		if (m_ECS.GetHierarchy().TryGetParent(entity, parent)) {

			// Calculate the world matrix
			auto parentTransform = GetMutableTransform(parent);
			transform->WorldTransformMatrix = parentTransform ? (parentTransform->WorldTransformMatrix * transform->LocalTransformMatrix) : transform->LocalTransformMatrix;
		}
		else {
			transform->WorldTransformMatrix = transform->LocalTransformMatrix;
		}

		transform->m_IsDirty = false;
		return transform;
	}

	void TransformManager::MarkDirty(EntityId entity, OptionalReference<TransformComponent> transform)
	{
		if (transform) {
			if (transform->m_IsDirty) return; // all children will already be dirty
			transform->m_IsDirty = true;
		}

		const auto& children = m_ECS.GetHierarchy().GetChildren(entity);
		for (auto& child : children) {
			MarkDirty(entity, m_ECS.GetMutableComponent<TransformComponent>(child));
		}
	}
}