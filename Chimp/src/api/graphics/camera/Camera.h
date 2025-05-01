#pragma once

#include "CameraMatrices.h"
#include "ICamera.h"

namespace Chimp {
	// Represents an orthographic camera in 3D space
	// Updating any property of the camera will automatically update the view / projection matrix
	class Camera : public ICamera {
	public:
		// Create a camera positioned at (0, 0, -1000) where +z is forward and -y is up and +x is right
		// The view will be a 1280x720 viewport positioned at (0, 0) with a clipping plane of 0.0 to 1000.0
		Camera();
		~Camera() = default;

		void SetPosition(const Vector3f& position) override;

		void SetUpVector(const Vector3f& up) override;

		void SetForwardVector(const Vector3f& forward) override;

		void SetRightVector(const Vector3f& right) override;

		void SetNormalizedVectors(const Vector3f& forward, const Vector3f& up, const Vector3f& right) override;
		void SetNormalizedVectors(const std::array<Vector3f, 3> forwardUpRight) override;

		void Rotate(const Quaternion& quat) override;

		// Set the top left of the view in screen space
		// This generally will be (0, 0). (unless split screen!!)
		void SetViewTopLeft(const Vector2f& topLeft);

		// Set the size of the viewport in screen space
		void SetViewSize(const Vector2f& size);

		// Set the clipping plane of the view.
		// This is the minimum and maximum z coordinates where objects will be visible. (zNear, zFar)
		void SetViewClippingPlane(const Vector2f& clippingPlane) override;

		[[nodiscard]] const Vector3f& GetPosition() const override;

		[[nodiscard]] const Vector3f GetTarget() const override;

		[[nodiscard]] const Vector3f& GetUpVector() const override;

		[[nodiscard]] const Vector3f& GetForwardVector() const override;

		[[nodiscard]] const Vector3f& GetRightVector() const override;

		// Get the top left of the view in screen space
		[[nodiscard]] const Vector2f& GetViewTopLeft() const;

		// Get the bottom right of the view in screen space
		[[nodiscard]] const Vector2f& GetViewBottomRight() const;

		// Get the size of the viewport in screen space
		[[nodiscard]] const Vector2f GetViewSize() const;

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

		Vector2f m_ViewTopLeft;
		Vector2f m_ViewBottomRight;
		Vector2f m_ViewClippingPlane;
	};
}