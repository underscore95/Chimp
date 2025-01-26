#include "api/graphics/camera/CameraMatrices.h"

namespace Chimp {
	CameraMatrices::CameraMatrices()
		: m_ViewMatrix(CreateIdentityMatrix()),
		m_ProjectionMatrix(CreateIdentityMatrix())
	{
	}

	void CameraMatrices::SetViewMatrix(const Matrix& viewMatrix)
	{
		m_ViewMatrix = viewMatrix;
	}

	void CameraMatrices::SetProjectionMatrix(const Matrix& projectionMatrix)
	{
		m_ProjectionMatrix = projectionMatrix;
	}

	const Matrix& CameraMatrices::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	const Matrix& CameraMatrices::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}
}