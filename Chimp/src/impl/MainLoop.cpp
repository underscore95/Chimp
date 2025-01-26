#include "MainLoop.h"
#include "api/Engine.h"

namespace Chimp {
	MainLoop::MainLoop(std::unique_ptr<Scene> entryScene,
		Engine& engine)
		: m_Engine(engine) {
		m_Engine.m_SceneManager = std::unique_ptr<SceneManager>(new SceneManager(std::move(entryScene), m_Engine));

		while (!m_Engine.GetSceneManager().m_CurrentScene->ShouldExit(m_Engine)) {
			m_Engine.GetTimeManager().Update();
			m_Engine.GetWindow().Update();
			m_Engine.GetSceneManager().Update();
			m_Engine.GetUpdateSubscriber().Update();
			m_Engine.GetResourceManager().Update();
			m_Engine.GetMusicPlayer().Update();

			m_Engine.GetWindow().UpdateEnd();

			m_Engine.GetRenderingManager().GetRenderer().StartDrawing();
			m_Engine.GetSceneManager().Render();
			m_Engine.GetUpdateSubscriber().Render();

			m_Engine.GetWindow().GetImGuiHandler().StartImGuiFrame();
			m_Engine.GetSceneManager().RenderUI();
			m_Engine.GetUpdateSubscriber().RenderUI();
			m_Engine.GetWindow().GetImGuiHandler().EndImGuiFrame();

			m_Engine.GetWindow().SwapBuffers();

			m_Engine.GetSceneManager().CheckForSceneChange();
		}
	}
}