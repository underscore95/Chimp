#include "api/graphics/camera/ICamera.h"

std::shared_ptr<Chimp::IBuffer> CreateCameraBuffer(Chimp::IRenderingManager& renderingManager)
{
	return renderingManager.CreateBuffer(
		sizeof(Chimp::Matrix),
		1,
		{
			Chimp::Usage::UpdateFrequency::OCCASIONAL,
			Chimp::Usage::Access::CPU_WRITE
		},
		Chimp::BindTarget::SHADER_BUFFER
	);
}

namespace Chimp {
	ICamera::ICamera(IRenderingManager& renderingManager) :
		m_Buffer(CreateCameraBuffer(renderingManager))
	{
	}

	[[nodiscard]] IBuffer& ICamera::GetBuffer()
	{
		return *m_Buffer;
	}
}