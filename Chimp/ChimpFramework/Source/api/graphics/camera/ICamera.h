#pragma once

#include "CameraMatrices.h"

namespace Chimp {
	class ICamera {
	public:
		// Set the position of the camera
		virtual	void SetPosition(const Vector3f& position) = 0;

		// Set the up vector of the camera, this will normalise (a copy of) the input vector
		virtual	void SetUpVector(const Vector3f& up) = 0;

		// Set the forward vector of the camera, this will normalise (a copy of) the input vector
		virtual	void SetForwardVector(const Vector3f& forward) = 0;

		// Set the right vector of the camera, this will normalise (a copy of) the input vector
		virtual	void SetRightVector(const Vector3f& right) = 0;

		// Set forward, up, right vectors of the camera all at once, they must already be normalised
		virtual	void SetNormalizedVectors(const Vector3f& forward, const Vector3f& up, const Vector3f& right) = 0;
		virtual	void SetNormalizedVectors(const std::array<Vector3f, 3> forwardUpRight) = 0;

		// Rotate forward, up, right vectors by this quaternion
		virtual void Rotate(const Quaternion& quat) = 0;

		// Set the clipping plane of the view.
		// This is the minimum and maximum z coordinates where objects will be visible. (zNear, zFar)
		virtual void SetViewClippingPlane(const Vector2f& clippingPlane) = 0;

		// Get the position of the camera
		[[nodiscard]] virtual const Vector3f& GetPosition() const = 0;

		// Equivalent to position + forward vector, get the position the camera is looking at
		[[nodiscard]] virtual const Vector3f GetTarget() const = 0;

		// Get the up vector of the camera
		[[nodiscard]] virtual const Vector3f& GetUpVector() const = 0;

		// Get the forward vector of the camera
		[[nodiscard]] virtual const Vector3f& GetForwardVector() const = 0;

		// Get the right vector of the camera
		[[nodiscard]] virtual const Vector3f& GetRightVector() const = 0;

		// Get the clipping plane of the view.
		// This is the minimum and maximum z coordinates where objects will be visible. (zNear, zFar)
		[[nodiscard]] virtual const Vector2f& GetViewClippingPlane() const = 0;

		[[nodiscard]] virtual const CameraMatrices& GetCameraMatrices() const = 0;
	};
}