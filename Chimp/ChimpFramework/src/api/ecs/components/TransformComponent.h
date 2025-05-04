#pragma once

#include "stdafx.h"
#include "api/utils/Maths.h"

namespace Chimp {

	class ECS;
	class TransformManager;

	struct TransformComponent {
		friend class ECS;
		friend class TransformManager;
	public:
		TransformComponent() : TransformComponent({}, CreateIdentityQuaternion(), {}) {}

		TransformComponent(const Vector3f& translation,
			const Vector3f& eulerRotationDegrees,
			const Vector3f& scale) : TransformComponent(translation, ToQuatRotation(eulerRotationDegrees), scale)
		{ }

		TransformComponent(const Vector3f& translation,
			const Quaternion& rotation,
			const Vector3f& scale) :
			LocalTranslation(translation),
			LocalRotation(rotation),
			LocalScale(scale)
		{
			m_IsDirty = true;
		}

	public:
		Vector3f LocalTranslation;
		Vector3f LocalScale;
		Quaternion LocalRotation;

		Matrix WorldTransformMatrix;
		Matrix LocalTransformMatrix;

		inline bool IsDirty() const { return m_IsDirty; }

	private:
		bool m_IsDirty;
	};
}