#include "api/graphics/camera/controllers/FreeCameraController.h"

namespace Chimp {
	FreeCameraController::FreeCameraController(ICamera& camera, InputManager& inputManager)
		: m_Camera(camera), m_InputManager(inputManager)
	{
	}

	void FreeCameraController::OnUpdate(float deltaTime)
	{
		// WASDQE to move position
		Vector3f translation;
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

	// Rotation
	Vector3f rotation;
	{
		if (m_InputManager.IsKeyDown(Keyboard::I)) {
			rotation.x++;
		}
		if (m_InputManager.IsKeyDown(Keyboard::K)) {
			rotation.x--;
		}
		if (m_InputManager.IsKeyDown(Keyboard::L)) {
			rotation.y--;
		}
		if (m_InputManager.IsKeyDown(Keyboard::J)) {
			rotation.y++;
		}
	}

	if (rotation != Vector3f()) {
		rotation = VectorNormalized(rotation) * ROTATION_SPEED * deltaTime;
		Quaternion quat = QuatRotation(rotation);
		m_Camera.Rotate(quat);
	}
}
}