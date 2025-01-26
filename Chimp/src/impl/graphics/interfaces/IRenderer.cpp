#include "api/graphics/IRenderer.h"

namespace Chimp
{
	Camera& IRenderer::GetDefaultCamera()
	{
		return *m_DefaultCamera;
	}

	void IRenderer::SetCamera(std::shared_ptr<ICamera> camera)
	{
		m_Camera = camera;
	}

	void IRenderer::Draw(const Mesh& mesh, const IShader& shader) const
	{
		for (const auto& section : mesh)
		{
			Draw(section, shader);
		}
	}

	const CameraMatrices& IRenderer::GetCameraMatrices() const
	{
		return m_Camera->GetCameraMatrices();
	}
}