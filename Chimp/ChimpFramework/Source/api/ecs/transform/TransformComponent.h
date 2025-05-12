#pragma once

#include "stdafx.h"
#include "api/utils/Maths.h"
#include "api/ecs/components/ComponentRegistry.h"
#include "api/ecs/transform/TransformManager.h"
#include "api/ecs/ECS.h"

namespace Chimp {

	struct TransformComponent {
		friend class ECS;
		friend class TransformManager;
	public:
		TransformComponent() : TransformComponent({}, CreateIdentityQuaternion(), { 1,1,1 }) {}

		TransformComponent(const Vector3f& translation,
			const Vector3f& eulerRotationDegrees,
			const Vector3f& scale) : TransformComponent(translation, ToQuatRotation(eulerRotationDegrees), scale)
		{
		}

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

	namespace Unused {
		class TransformComponentRegister : public ComponentRegister<TransformComponent> {
		public:
			void RenderInspectorUI(EntityId id, TransformComponent& comp) override {
				TransformComponent copy = comp;
				bool isDirty = copy.IsDirty();

				// Translation
				auto translationLabel = std::format("Translation##{}", (long)id);
				isDirty |= ImGui::InputFloat3(translationLabel.c_str(), &copy.LocalTranslation.x);

				// Rotation
				Chimp::Vector3f rotation = ToEulerRotation(copy.LocalRotation);
				auto rotationLabel = std::format("Rotation##{}{}", (long)id, rotation.x * rotation.y * rotation.z);
				if (ImGui::InputFloat3(rotationLabel.c_str(), &rotation.x)) {
					copy.LocalRotation = ToQuatRotation(rotation);
					isDirty = true;
				}

				// Scale
				auto scaleLabel = std::format("Scale##{}", (long)id);
				isDirty |= ImGui::InputFloat3(scaleLabel.c_str(), &copy.LocalScale.x);

				GetECS().GetTransformManager().SetTranslationRotationScale(id, copy.LocalTranslation, copy.LocalRotation, copy.LocalScale);
			}
		};
		COMPONENT_REGISTER(TransformComponentRegister);
	}
}