#pragma once

#include "CameraMatrices.h"
#include "ICamera.h"

namespace Chimp {
	// Represents an orthographic camera in 3D space
	// if this is too high level or you need a perspective camera, see CameraMatrices
	// Updating any property of the camera will automatically update the view / projection matrix
	class Camera : public ICamera {
	public:
		// Create a camera positioned at (0, 0, -1) where +z is forward and -y is up and +x is right
		// The view will be a 1280x720 viewport positioned at (0, 0) with a clipping plane of 0.0 to 1000.0
		Camera();
		~Camera() = default;

		// Set the position of the camera
		void SetPosition(const Vector3f& position);

		// Set the up vector of the camera, this will normalise (a copy of) the input vector
		void SetUpVector(const Vector3f& up);

		// Set the forward vector of the camera, this will normalise (a copy of) the input vector
		void SetForwardVector(const Vector3f& forward);

		// Set the right vector of the camera, this will normalise (a copy of) the input vector
		void SetRightVector(const Vector3f& right);

		// Set forward, up, right vectors of the camera all at once, they must already be normalised
		void SetNormalizedVectors(const Vector3f& forward, const Vector3f& up, const Vector3f& right);
		void SetNormalizedVectors(const std::array<Vector3f, 3> forwardUpRight);

		// Set the top left of the view in screen space
		// This generally will be (0, 0). (unless split screen!!)
		void SetViewTopLeft(const Vector2f& topLeft);

		// Set the size of the viewport in screen space
		void SetViewSize(const Vector2f& size);

		// Set the clipping plane of the view.
		// This is the minimum and maximum z coordinates where objects will be visible. (zNear, zFar)
		void SetViewClippingPlane(const Vector2f& clippingPlane);

		// Get the position of the camera
		[[nodiscard]] const Vector3f& GetPosition() const;

		// Equivalent to position + forward vector, get the position the camera is looking at
		[[nodiscard]] const Vector3f GetTarget() const;

		// Get the up vector of the camera
		[[nodiscard]] const Vector3f& GetUpVector() const;

		// Get the forward vector of the camera
		[[nodiscard]] const Vector3f& GetForwardVector() const;

		// Get the right vector of the camera
		[[nodiscard]] const Vector3f& GetRightVector() const;

		// Get the top left of the view in screen space
		[[nodiscard]] const Vector2f& GetViewTopLeft() const;

		// Get the bottom right of the view in screen space
		[[nodiscard]] const Vector2f& GetViewBottomRight() const;

		// Get the size of the viewport in screen space
		[[nodiscard]] const Vector2f GetViewSize() const;

		// Get the clipping plane of the view.
		// This is the minimum and maximum z coordinates where objects will be visible. (zNear, zFar)
		[[nodiscard]] const Vector2f& GetViewClippingPlane() const;

		// Get the camera matrices
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