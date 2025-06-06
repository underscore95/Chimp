#pragma once

#include "stdafx.h"
#include "Keys.h"
#include "api/utils/preprocessor/CopyAndMove.h"

namespace Chimp {
	class WindowGLFW;
	class InputManager {
		friend class IWindow;
		friend class WindowGLFW;
		DISABLE_COPY_AND_MOVE(InputManager);
	private:
		InputManager();

		void OnKeyDown(Keyboard::Key key);
		void OnKeyUp(Keyboard::Key key);
		void OnMouseDown(Mouse::Button button);
		void OnMouseUp(Mouse::Button button);
		void SetMousePosition(const Vector2f &position);

		// Should be called at the end of every frame.
		void UpdateEnd();
	public:
		[[nodiscard]] bool IsKeyDown(Keyboard::Key key) const;
		[[nodiscard]] bool IsKeyPressed(Keyboard::Key key) const;
		[[nodiscard]] bool IsMouseButtonDown(Mouse::Button button) const;
		[[nodiscard]] bool IsMouseButtonPressed(Mouse::Button button) const;
		[[nodiscard]] Vector2f GetMousePosition() const;

	private:
		std::array<bool, Keyboard::LAST_KEY> m_KeysDown;
		std::array<bool, Keyboard::LAST_KEY> m_KeysPressed;
		std::array<bool, Mouse::LAST_BUTTON> m_MouseButtonsDown;
		std::array<bool, Mouse::LAST_BUTTON> m_MouseButtonsPressed;
		Vector2f m_MousePosition;
	};
}