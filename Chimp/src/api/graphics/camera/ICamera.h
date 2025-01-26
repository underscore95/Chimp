#pragma once

#include "CameraMatrices.h"

namespace Chimp {
	class ICamera {
	public:
		[[nodiscard]] virtual const CameraMatrices& GetCameraMatrices() const = 0;
	};
}