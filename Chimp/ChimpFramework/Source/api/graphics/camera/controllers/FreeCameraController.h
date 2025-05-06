#pragma once

#include "../Camera.h"
#include "api/window/InputManager.h"

namespace Chimp {
	class FreeCameraController {
	public:
		FreeCameraController(ICamera& camera, InputManager& inputManager);

		void OnUpdate(float deltaTime);

	private:
		ICamera& m_Camera;
		InputManager& m_InputManager;
		static constexpr float MOVEMENT_SPEED = 20.0f;
		static constexpr float ROTATION_SPEED = 1.0f;
	};
}