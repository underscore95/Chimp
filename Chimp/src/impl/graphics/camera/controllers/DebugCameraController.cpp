#include "api/graphics/camera/controllers/DebugCameraController.h"

namespace Chimp {
	DebugCameraController::DebugCameraController(Camera& camera, InputManager& inputManager)
		: m_Camera(camera), m_InputManager(inputManager)
	{
	}

	void DebugCameraController::OnUpdate(float deltaTime)
	{
		// Arrow keys to change rotation
		// note this looks weird if orthographic projection is used
		Vector3f deltaRotation;
		{
			if (m_InputManager.IsKeyDown(Keyboard::U))
			{
				deltaRotation.x += 1;
			}
			if (m_InputManager.IsKeyDown(Keyboard::J))
			{
				deltaRotation.x -= 1;
			}
			if (m_InputManager.IsKeyDown(Keyboard::K))
			{
				deltaRotation.y -= 1;
			}
			if (m_InputManager.IsKeyDown(Keyboard::I))
			{
				deltaRotation.y += 1; 
			}
			if (m_InputManager.IsKeyDown(Keyboard::O))
			{
				deltaRotation.z += 1; 
			}
			if (m_InputManager.IsKeyDown(Keyboard::L))
			{
				deltaRotation.z -= 1; 
			}

		}

		if (deltaRotation != Vector3f()) {
			m_YawPitchRoll += VectorNormalized(deltaRotation) * ROTATION_SPEED * deltaTime;
		}

		m_Camera.SetNormalizedVectors(GetForwardUpRightVectors(m_YawPitchRoll));

		// WASDQE to move position
		Vector3f translation = Vector3f();
		{
			if (m_InputManager.IsKeyDown(Keyboard::W))
			{
				translation += m_Camera.GetForwardVector();
			}
			if (m_InputManager.IsKeyDown(Keyboard::S))
			{
				translation -= m_Camera.GetForwardVector();
			}
			if (m_InputManager.IsKeyDown(Keyboard::A))
			{
				translation -= m_Camera.GetRightVector();
			}
			if (m_InputManager.IsKeyDown(Keyboard::D))
			{
				translation += m_Camera.GetRightVector();
			}
			if (m_InputManager.IsKeyDown(Keyboard::Q))
			{
				translation -= m_Camera.GetUpVector();
			}
			if (m_InputManager.IsKeyDown(Keyboard::E))
			{
				translation += m_Camera.GetUpVector();
			}
		}

		if (translation != Vector3f()) {
			translation = VectorNormalized(translation) * MOVEMENT_SPEED * deltaTime;
			m_Camera.SetPosition(m_Camera.GetPosition() + translation);
		}

		// G to reset rotation, h to reset position
		if (m_InputManager.IsKeyDown(Keyboard::G))
		{
			m_YawPitchRoll = { PI, 0.0f, 0.0f };
			m_Camera.SetNormalizedVectors(GetForwardUpRightVectors(m_YawPitchRoll));
		}

		if (m_InputManager.IsKeyDown(Keyboard::H))
		{
			m_Camera.SetPosition(Vector3f());
		}
	}
}