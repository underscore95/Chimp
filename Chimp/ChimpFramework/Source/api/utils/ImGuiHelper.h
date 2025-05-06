#pragma once

#include "stdafx.h"

namespace Chimp {
	class Engine;
	class ImGuiHelper {
		friend class Engine;
	private:
		ImGuiHelper() = default;
	public:
		~ImGuiHelper() = default;

		void CenteredTextHorizontally(const std::string& text);
		void CenteredTextVertically(const std::string& text);
		void CenteredText(const std::string& text);
	};
}