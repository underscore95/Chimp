#include "api/graphics/camera/CameraPerspective.h"

namespace Chimp {
	CameraPerspective::CameraPerspective() :
		m_Position(0.0f, 0.0f, 1.0f),
		m_UpVector(0.0f, 1.0f, 0.0f),
		m_ForwardVector(0.0f, 0.0f, -1.0f),
		m_RightVector(1.0f, 0.0f, 0.0f),
		m_AspectRatio(16.0f / 9.0f),
		m_FOV(50),
		m_ViewClippingPlane(0.1f, 1000.0f)
	{
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void CameraPerspective::SetPosition(const Vector3f& position)
	{
		m_Position = position;
		UpdateViewMatrix();
	}

	void CameraPerspective::SetUpVector(const Vector3f& up)
	{
		m_UpVector = VectorNormalized(up);
		UpdateViewMatrix();
	}

	void CameraPerspective::SetForwardVector(const Vector3f& forward)
	{
		m_ForwardVector = VectorNormalized(forward);
		UpdateViewMatrix();
	}

	void CameraPerspective::SetRightVector(const Vector3f& right)
	{
		m_RightVector = VectorNormalized(right);
		UpdateViewMatrix();
	}

	void CameraPerspective::SetNormalizedVectors(const Vector3f& forward, const Vector3f& up, const Vector3f& right)
	{
		m_ForwardVector = forward;
		m_UpVector = up;
		m_RightVector = right;
		UpdateViewMatrix();
	}

	void CameraPerspective::SetNormalizedVectors(const std::array<Vector3f, 3> forwardUpRight)
	{
		SetNormalizedVectors(forwardUpRight[0], forwardUpRight[1], forwardUpRight[2]);
	}

	void CameraPerspective::Rotate(const Quaternion& quat)
	{
		m_UpVector *= quat;
		m_RightVector *= quat;
		m_ForwardVector *= quat;
		
		UpdateViewMatrix();
	}

	void CameraPerspective::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		UpdateProjectionMatrix();
	}

	void CameraPerspective::SetViewClippingPlane(const Vector2f& clippingPlane)
	{
		assert(clippingPlane.x < clippingPlane.y);
		m_ViewClippingPlane = clippingPlane;
		UpdateProjectionMatrix();
	}

	const Vector3f& CameraPerspective::GetPosition() const
	{
		return m_Position;
	}

	const Vector3f CameraPerspective::GetTarget() const
	{
		return m_Position + m_ForwardVector;
	}

	const Vector3f& CameraPerspective::GetUpVector() const
	{
		return m_UpVector;
	}

	const Vector3f& CameraPerspective::GetForwardVector() const
	{
		return m_ForwardVector;
	}

	const Vector3f& CameraPerspective::GetRightVector() const
	{
		return m_RightVector;
	}

	const Vector2f& CameraPerspective::GetViewClippingPlane() const
	{
		return m_ViewClippingPlane;
	}

	const CameraMatrices& CameraPerspective::GetCameraMatrices() const
	{
		return m_CameraMatrices;
	}

	void CameraPerspective::UpdateViewMatrix()
	{
		m_CameraMatrices.SetViewMatrix(CreateViewMatrix(m_Position, m_Position + m_ForwardVector, m_UpVector));
	}

	void CameraPerspective::UpdateProjectionMatrix()
	{
		m_CameraMatrices.SetProjectionMatrix(CreatePerspectiveProjectionMatrix(
			m_FOV,
			m_AspectRatio,
			m_ViewClippingPlane.x,
			m_ViewClippingPlane.y
		));
	}
}