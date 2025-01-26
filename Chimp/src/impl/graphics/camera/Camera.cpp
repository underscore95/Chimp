#include "api/graphics/camera/Camera.h"

namespace Chimp {
	Camera::Camera() :
		m_Position(0.0f, 0.0f, -1000.0f),
		m_UpVector(0.0f, -1.0f, 0.0f),
		m_ForwardVector(0.0f, 0.0f, 1.0f),
		m_RightVector(1.0f, 0.0f, 0.0f),
		m_ViewTopLeft(0.0f, 0.0f),
		m_ViewBottomRight(1280.0f, 720.0f),
		m_ViewClippingPlane(-1000.0f, 100000.0f)
	{
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void Camera::SetPosition(const Vector3f& position)
	{
		m_Position = position;
		UpdateViewMatrix();
	}

	void Camera::SetUpVector(const Vector3f& up)
	{
		m_UpVector = VectorNormalized(up);
		UpdateViewMatrix();
	}

	void Camera::SetForwardVector(const Vector3f& forward)
	{
		m_ForwardVector = VectorNormalized(forward);
		UpdateViewMatrix();
	}

	void Camera::SetRightVector(const Vector3f& right)
	{
		m_RightVector = VectorNormalized(right);
		UpdateViewMatrix();
	}

	void Camera::SetNormalizedVectors(const Vector3f& forward, const Vector3f& up, const Vector3f& right)
	{
		m_ForwardVector = forward;
		m_UpVector = up;
		m_RightVector = right;
		UpdateViewMatrix();
	}

	void Camera::SetNormalizedVectors(const std::array<Vector3f, 3> forwardUpRight)
	{
		SetNormalizedVectors(forwardUpRight[0], forwardUpRight[1], forwardUpRight[2]);
	}

	void Camera::SetViewTopLeft(const Vector2f& topLeft)
	{
		m_ViewTopLeft = topLeft;
		UpdateProjectionMatrix();
	}

	void Camera::SetViewSize(const Vector2f& size)
	{
		assert(size.x > 0 && size.y > 0);
		m_ViewBottomRight = m_ViewTopLeft + size;
		UpdateProjectionMatrix();
	}

	void Camera::SetViewClippingPlane(const Vector2f& clippingPlane)
	{
		assert(clippingPlane.x < clippingPlane.y);
		m_ViewClippingPlane = clippingPlane;
		UpdateProjectionMatrix();
	}

	const Vector3f& Camera::GetPosition() const
	{
		return m_Position;
	}

	const Vector3f Camera::GetTarget() const
	{
		return m_Position + m_ForwardVector;
	}

	const Vector3f& Camera::GetUpVector() const
	{
		return m_UpVector;
	}

	const Vector3f& Camera::GetForwardVector() const
	{
		return m_ForwardVector;
	}

	const Vector3f& Camera::GetRightVector() const
	{
		return m_RightVector;
	}

	const Vector2f& Camera::GetViewTopLeft() const
	{
		return m_ViewTopLeft;
	}

	const Vector2f& Camera::GetViewBottomRight() const
	{
		return m_ViewBottomRight;
	}

	const Vector2f Camera::GetViewSize() const
	{
		return m_ViewBottomRight - m_ViewTopLeft;
	}

	const Vector2f& Camera::GetViewClippingPlane() const
	{
		return m_ViewClippingPlane;
	}

	const CameraMatrices& Camera::GetCameraMatrices() const
	{
		return m_CameraMatrices;
	}

	void Camera::UpdateViewMatrix()
	{
		m_CameraMatrices.SetViewMatrix(CreateViewMatrix(m_Position, m_Position + m_ForwardVector, m_UpVector));
	}

	void Camera::UpdateProjectionMatrix()
	{
		m_CameraMatrices.SetProjectionMatrix(CreateOrthographicProjectionMatrix(
			m_ViewTopLeft.x,
			m_ViewBottomRight.x,
			m_ViewBottomRight.y,
			m_ViewTopLeft.y,
			m_ViewClippingPlane.x,
			m_ViewClippingPlane.y
		));
	}
}