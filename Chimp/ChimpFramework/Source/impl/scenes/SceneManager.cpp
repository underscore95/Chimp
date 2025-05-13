#include "api/scenes/SceneManager.h"
#include "api/Engine.h"

namespace Chimp {
	SceneManager::SceneManager(std::unique_ptr<Scene> entryScene, Chimp::Engine& engine)
		: m_CurrentScene(std::move(entryScene)),
		m_Engine(engine) {
		m_Engine.GetResourceManager().LoadRequiredResources();
		m_CurrentScene->OnInit();
		m_CurrentScene->m_HasInitialized = true;
		m_CurrentScene->OnActivate(nullptr);
	}

	SceneManager::~SceneManager()
	{
		m_CurrentScene->OnDeactivate();
		m_Engine.GetResourceManager().UnloadUnusedResources();
	}

	void SceneManager::QueueSceneChange(std::unique_ptr<Scene> newScene)
	{
		m_NextScene = std::move(newScene);
	}

	void SceneManager::ChangeScene(std::unique_ptr<Scene> newScene)
	{
		QueueSceneChange(std::move(newScene));
		CheckForSceneChange();
	}

	void SceneManager::Update()
	{
		m_CurrentScene->OnUpdate();
	}

	void SceneManager::Render()
	{
		m_Engine.GetRenderingManager().BeginDebugGroup("Scene Render");
		m_CurrentScene->OnRender();
		m_Engine.GetRenderingManager().EndDebugGroup();
	}

	void SceneManager::RenderUI()
	{
		m_Engine.GetRenderingManager().BeginDebugGroup("Scene UI Render");
		m_CurrentScene->OnRenderUI();
		m_Engine.GetRenderingManager().EndDebugGroup();
	}

	bool SceneManager::CheckForSceneChange()
	{
		if (m_NextScene != nullptr) [[unlikely]] {
			m_CurrentScene->OnDeactivate();
			std::unique_ptr<Scene> previousScene = std::move(m_CurrentScene);
			m_CurrentScene = std::move(m_NextScene);
			m_CurrentScene->OnActivate(std::move(previousScene));
			m_Engine.GetResourceManager().UnloadUnusedResources();
			m_Engine.GetResourceManager().LoadRequiredResources();
			if (!m_CurrentScene->m_HasInitialized) {
				m_CurrentScene->OnInit();
				m_CurrentScene->m_HasInitialized = true;
			}
			return true;
		}
		return false;
	}


}