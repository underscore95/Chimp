#include "api/window/InputManager.h"

namespace Chimp {
	InputManager::InputManager() :
		m_KeysDown(),
		m_KeysPressed(),
		m_MouseButtonsDown(),
		m_MouseButtonsPressed(),
		m_MousePosition(0.0f, 0.0f)
	{
		m_KeysDown.fill(false);
		m_KeysPressed.fill(false);
		m_MouseButtonsDown.fill(false);
		m_MouseButtonsPressed.fill(false);
	}

	void InputManager::OnKeyDown(Chimp::Keyboard::Key key)
	{
		if (!m_KeysDown[key]) {
			m_KeysPressed[key] = true;
		}

		m_KeysDown[key] = true;
	}

	void InputManager::OnKeyUp(Chimp::Keyboard::Key key)
	{
		m_KeysDown[key] = false;
	}

	void InputManager::OnMouseDown(Mouse::Button button)
	{
		if (!m_MouseButtonsDown[button]) {
			m_MouseButtonsPressed[button] = true;
		}

		m_MouseButtonsDown[button] = true;

	}

	void InputManager::OnMouseUp(Mouse::Button button)
	{
		m_MouseButtonsDown[button] = false;
	}

	void InputManager::SetMousePosition(const Vector2f& position)
	{
		m_MousePosition = position;
	}

	void InputManager::UpdateEnd()
	{
		m_KeysPressed.fill(false);
		m_MouseButtonsPressed.fill(false);
	}

	bool InputManager::IsKeyDown(Chimp::Keyboard::Key key) const
	{
		return m_KeysDown[key];
	}

	bool InputManager::IsKeyPressed(Chimp::Keyboard::Key key) const
	{
		return m_KeysPressed[key];
	}

	bool InputManager::IsMouseButtonDown(Mouse::Button button) const
	{
		return m_MouseButtonsDown[button];
	}

	bool InputManager::IsMouseButtonPressed(Mouse::Button button) const
	{
		return m_MouseButtonsPressed[button];
	}
	Vector2f InputManager::GetMousePosition() const
	{
		return m_MousePosition;
	}
}