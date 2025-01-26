#include "api/window/IWindow.h"

namespace Chimp {
	void IWindow::UpdateEnd()
	{
		m_InputManager.UpdateEnd();
	}

	ImGuiHandler& IWindow::GetImGuiHandler()
	{
		return m_ImGuiHandler;
	}

	bool IWindow::HasResized()
	{
		const auto size = GetSize();
		if (size != m_OutdatedSize)
		{
			m_OutdatedSize = size;
			return true;
		}
		return false;
	}

	WindowStatus IWindow::GetStatus() const
	{
		return m_Status;
	}

	InputManager& IWindow::GetInputManager()
	{
		return m_InputManager;
	}
}