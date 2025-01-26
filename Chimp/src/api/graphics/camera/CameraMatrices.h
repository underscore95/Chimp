#pragma once

#include "stdafx.h"

namespace Chimp {
	// Contains the view and projection matrices for a camera
	class CameraMatrices {
	public:
		CameraMatrices();
		~CameraMatrices() = default;

		void SetViewMatrix(const Matrix& viewMatrix);
		void SetProjectionMatrix(const Matrix& projectionMatrix);

		[[nodiscard]] const Matrix& GetViewMatrix() const;
		[[nodiscard]] const Matrix& GetProjectionMatrix() const;

	private:
		Matrix m_ViewMatrix;
		Matrix m_ProjectionMatrix;
	};
}