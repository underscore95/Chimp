#pragma once

#include "stdafx.h"
#include "api/utils/Maths.h"

namespace Chimp {
	struct TransformComponent {
		TransformComponent() = default;
		TransformComponent(const Vector3f& translation, const Vector3f& rotation, const Vector3f& scale)
			: m_Transform{ translation, rotation, scale } {
			UpdateTransform();
		}
		TransformComponent(const Transform& transform)
			: m_Transform(transform) {
			UpdateTransform();
		}

		const Vector3f& GetTranslation() const { return m_Transform.Translation; }
		const Vector3f& GetRotation() const { return m_Transform.Rotation; }
		const Vector3f& GetScale() const { return m_Transform.Scale; }

		void Move(const float x, const float y, const float z) {
			SetTranslation(m_Transform.Translation + Vector3f{ x, y, z });
		}
		void SetTranslation(const float x, const float y, const float z) {
			SetTranslation({ x, y, z });
		}
		void SetTranslation(const Vector3f& translation) {
			m_Transform.Translation = translation;
			UpdateTransform();
		}
		void SetTranslationXY(const Vector2f& translation) {
			SetTranslation({ translation.x, translation.y, m_Transform.Translation.z });
		}
		// yaw, pitch, roll in radians
		void SetRotation(const Vector3f& rotation) {
			m_Transform.Rotation = rotation;
			UpdateTransform();
		}
		void SetYaw(const float yaw) {
			m_Transform.Rotation.y = yaw;
			UpdateTransform();
		}
		void SetPitch(const float pitch) {
			m_Transform.Rotation.x = pitch;
			UpdateTransform();
		}
		void SetRoll(const float roll) {
			m_Transform.Rotation.z = roll;
			UpdateTransform();
		}
		void SetScale(const Vector3f& scale) {
			m_Transform.Scale = scale;
			UpdateTransform();
		}

		const Matrix& GetTransformMatrix() const { return m_TransformMatrix; }
		const Transform& GetTransform() const { return m_Transform; } // Can't be changed since it wouldn't update our matrix

	private:
		void UpdateTransform() {
			// offset for bottom left origin
			m_TransformMatrix = m_Transform.CreateTransformMatrix(
				{
					m_Transform.Scale.x * 0.5f,
					m_Transform.Scale.y * -0.5f,
					0.0f
				}
			);
		}

	private:
		Transform m_Transform;
		Matrix m_TransformMatrix = CreateIdentityMatrix();
	};
}