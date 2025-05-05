#pragma once

#include "stdafx.h"

namespace Chimp {
	class IWindow;
	class ImGuiHandler {
		friend class IWindow;
	private:
		ImGuiHandler() = default;

	public:
		~ImGuiHandler();

		void Init(IWindow& window);

		void StartImGuiFrame();
		void EndImGuiFrame();
	};
}