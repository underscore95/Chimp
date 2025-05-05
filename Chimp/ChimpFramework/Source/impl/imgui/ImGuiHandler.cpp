#include "api/imgui/ImGuiHandler.h"

#ifdef CHIMP_GLFW
#include "impl/window/WindowGLFW.h"
#include "imgui_impl_glfw.h"
#endif

#ifdef CHIMP_OPENGL
#include "impl/graphics/opengl/RenderingManager.h"
#include "imgui_impl_opengl3.h"
#endif

#include "Loggers.h"

namespace Chimp {
	void ImGuiHandler::Init(IWindow& window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();

#if defined(CHIMP_OPENGL) && defined(CHIMP_GLFW)
		auto glfwWindow = static_cast<WindowGLFW&>(window).GetGLFWWindow();
		assert(glfwWindow != nullptr);

		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
		ImGui_ImplOpenGL3_Init("#version 130");
		Loggers::Main().Info("ImGui initialized for OpenGL and GLFW");
#else
		Loggers::Main().Error("ImGui failed to initialize: missing libraries. See impl/imgui/ImGuiHandler.cpp");
#endif
	}

	ImGuiHandler::~ImGuiHandler()
	{
#if defined(CHIMP_OPENGL) && defined(CHIMP_GLFW)
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
#endif
		ImGui::DestroyContext();
	}

	void ImGuiHandler::StartImGuiFrame()
	{
#if defined(CHIMP_OPENGL) && defined(CHIMP_GLFW)
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
#endif
		ImGui::NewFrame();
	}

	void ImGuiHandler::EndImGuiFrame()
	{
		ImGui::Render();
#if defined(CHIMP_OPENGL) && defined(CHIMP_GLFW)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
	}
}