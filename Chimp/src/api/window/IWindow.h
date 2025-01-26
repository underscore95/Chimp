#pragma once

#include "stdafx.h"
#include "InputManager.h"
#include "api/imgui/ImGuiHandler.h"

namespace Chimp {
	enum class WindowStatus {
		 SHOULD_CLOSE, OPEN, FAILED_INITIALIZATION
	};

	class MainLoop;
	class IWindow {
		friend class MainLoop;
		DISABLE_COPY(IWindow);
	protected:
		IWindow() = default;

	public:
		~IWindow() = default;

	protected:
		// Will be called at the beginning of every frame.
		// This function should poll any events and update m_ShouldClose
		virtual void Update() = 0;

		// Will be called at the end of every frame.
		void UpdateEnd();

		// Will be called after rendering of the frame is complete.
		// This function should swap the front and back buffers.
		virtual void SwapBuffers() = 0;

	public:
		ImGuiHandler& GetImGuiHandler();

		// Returns the size of the window.
		[[nodiscard]] virtual Vector2f GetSize() const = 0;

		// Sets the size of the window.
		virtual void SetSize(const Vector2f& size) = 0;

		// Sets the title of the window.
		virtual void SetTitle(const std::string& title) = 0;

		// Set if the window is resizable.
		virtual void SetResizable(bool resizable) = 0;

		// Returns true if the window has been resized since this method was last called.
		// Note that this method will return true on the first call. (opening the window counts as a resize)
		[[nodiscard]] bool HasResized();

		// Returns the current state of the window
		[[nodiscard]] WindowStatus GetStatus() const;

		// Returns the InputManager for this window.
		[[nodiscard]] InputManager& GetInputManager();

	protected:
		WindowStatus m_Status = WindowStatus::FAILED_INITIALIZATION;
		InputManager m_InputManager;
		Vector2f m_OutdatedSize{};
		ImGuiHandler m_ImGuiHandler;
	};
}