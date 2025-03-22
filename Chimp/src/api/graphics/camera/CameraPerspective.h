
#pragma once

#include "CameraMatrices.h"
#include "ICamera.h"

namespace Chimp {
	// Represents an perspective camera in 3D space
	// Updating any property of the camera will automatically update the view / projection matrix
	class CameraPerspective : public ICamera {
	public:
		// Create a camera positioned at (0, 0, 1) where -z is forward and +y is up and +x is right
		// The FOV will be 50 degrees, aspect ratio 16:9 and clipping plane 0.1 to 1000
		CameraPerspective();
		~CameraPerspective() = default;

		void SetPosition(const Vector3f& position) override;

		void SetUpVector(const Vector3f& up) override;

		void SetForwardVector(const Vector3f& forward) override;

		void SetRightVector(const Vector3f& right) override;

		void SetNormalizedVectors(const Vector3f& forward, const Vector3f& up, const Vector3f& right) override;
		void SetNormalizedVectors(const std::array<Vector3f, 3> forwardUpRight) override;

		void Rotate(const Quaternion& quat) override;

		// Set the aspect ratio
		void SetAspectRatio(float aspectRatio);

		// Set the clipping plane of the view.
		// This is the minimum and maximum z coordinates where objects will be visible. (zNear, zFar)
		void SetViewClippingPlane(const Vector2f& clippingPlane);

		[[nodiscard]] const Vector3f& GetPosition() const override;

		[[nodiscard]] const Vector3f GetTarget() const override;

		[[nodiscard]] const Vector3f& GetUpVector() const override;

		[[nodiscard]] const Vector3f& GetForwardVector() const override;

		[[nodiscard]] const Vector3f& GetRightVector() const override;

		[[nodiscard]] const Vector2f& GetViewClippingPlane() const override;

		[[nodiscard]] const CameraMatrices& GetCameraMatrices() const override;

	private:
		// Update the view matrix based on the current camera properties
		void UpdateViewMatrix();

		// Update the projection matrix based on the current camera properties
		void UpdateProjectionMatrix();

	private:
		CameraMatrices m_CameraMatrices;

		Vector3f m_Position;
		Vector3f m_UpVector;
		Vector3f m_ForwardVector;
		Vector3f m_RightVector;

	float m_AspectRatio;
		float m_FOV;
		Vector2f m_ViewClippingPlane;
	};
}