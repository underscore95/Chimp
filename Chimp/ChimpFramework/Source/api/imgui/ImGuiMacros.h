#pragma once

#include "api/Engine.h"

namespace Chimp::Unused {
	class ImGuiContextSetter {
	public:
		ImGuiContextSetter() {
			ImGuiContext* context = Engine::GetEngine().GetWindow().GetImGuiHandler().GetImGuiContext();
			assert(context);
			ImGui::SetCurrentContext(context);
		}
	};
}

// Set the ImGui context for the current domain, can be called repeatedly
#define CHIMP_SET_IMGUI_CONTEXT_IN_DOMAIN()                          \
	{ Chimp::Unused::ImGuiContextSetter _imguiContextSetter; assert(&_imguiContextSetter); }

// Set ImGui context once per function scope
#define CHIMP_SET_IMGUI_CONTEXT_IN_DOMAIN_ONCE()                     \
	{ static Chimp::Unused::ImGuiContextSetter _imguiContextSetterOnce; assert(&_imguiContextSetterOnce); }