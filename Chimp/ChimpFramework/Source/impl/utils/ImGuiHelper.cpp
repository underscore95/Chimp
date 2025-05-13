#include "api/utils/ImGuiHelper.h"

namespace Chimp {
	void ImGuiHelper::CenteredTextHorizontally(const std::string& text) {
		const auto textSize = ImGui::CalcTextSize(text.c_str());
		const auto windowSize = ImGui::GetWindowSize();
		ImGui::SetCursorPosX((windowSize.x - textSize.x) / 2);
		ImGui::Text(text.c_str());
	}

	void ImGuiHelper::CenteredTextVertically(const std::string& text) {
		const auto textSize = ImGui::CalcTextSize(text.c_str());
		const auto windowSize = ImGui::GetWindowSize();
		ImGui::SetCursorPosY((windowSize.y - textSize.y) / 2);
		ImGui::Text(text.c_str());
	}

	void ImGuiHelper::CenteredText(const std::string& text) {
		const auto textSize = ImGui::CalcTextSize(text.c_str());
		const auto windowSize = ImGui::GetWindowSize();
		const ImVec2 windowCenter = { (windowSize.x - textSize.x) / 2, (windowSize.y - textSize.y) / 2 };
		ImGui::SetCursorPos(windowCenter);
		ImGui::Text(text.c_str());
	}
}