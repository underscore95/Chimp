#pragma once

#include "api/window/IWindow.h"

namespace Chimp {
	class WindowGLFW : public IWindow {
	public:
		WindowGLFW();
		~WindowGLFW();

		GLFWwindow* GetGLFWWindow() const;
	protected:
		void Update() override;
		void SwapBuffers() override;

		Vector2f GetSize() const override;
		void SetSize(const Vector2f& size) override;
		void SetTitle(const std::string& title) override;
		void SetResizable(bool resizable) override;

	private:
		GLFWwindow* m_Window = nullptr;

	private:
		static bool GLFWInitialized;
		static void InitGLFW();
	};
}