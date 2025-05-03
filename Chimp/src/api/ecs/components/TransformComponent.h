#pragma once

#include "stdafx.h"
#include "api/utils/Maths.h"

namespace Chimp {

	struct TransformComponent {
		TransformComponent() : TransformComponent({}, {}, {}) {}

		TransformComponent(const Vector3f& translation, 
			const Vector3f& rotation, 
			const Vector3f& scale) :
		m_Translation(translation),
			m_Rotation(ToQuatRotation(rotation)),
			m_Scale(scale)
		{
			UpdateTransform();
		}

		const Vector3f& GetTranslation() const { return m_Translation; }
		const Quaternion& GetRotation() const { return m_Rotation; }
		const Vector3f& GetScale() const { return m_Scale; }

		void Move(const float x, const float y, const float z) {
			SetTranslation(m_Translation + Vector3f{ x, y, z });
		}
		void SetTranslation(const float x, const float y, const float z) {
			SetTranslation({ x, y, z });
		}
		void SetTranslation(const Vector3f& translation) {
			m_Translation = translation;
			UpdateTransform();
		}
		void SetTranslationXY(const Vector2f& translation) {
			SetTranslation({ translation.x, translation.y, m_Translation.z });
		}
		void SetRotation(const Quaternion& rotation) {
			m_Rotation = rotation;
			UpdateTransform();
		}
		void SetScale(const Vector3f& scale) {
			m_Scale = scale;
			UpdateTransform();
		}

		const Matrix& GetTransformMatrix() const { return m_TransformMatrix; }

	private:
		void UpdateTransform() {
			m_TransformMatrix = 
				CreateTranslationMatrix(m_Translation) *
				CreateRotationMatrix(m_Rotation) *
				CreateScaleMatrix(m_Scale);
		}

	private:
		Vector3f m_Translation;
		Vector3f m_Scale;
		Quaternion m_Rotation;
		Matrix m_TransformMatrix;
	};
}