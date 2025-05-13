#pragma once

#include "Scene.h"

namespace Chimp {
	class MainLoop;
	class SceneManager {
		friend class MainLoop;
	private:
		SceneManager(std::unique_ptr<Scene> entryScene, Chimp::Engine& engine);

	public:
		~SceneManager();

		// Switch to the new scene after the current frame
		// newScene cannot be nullptr
		void QueueSceneChange(std::unique_ptr<Scene> newScene);

		// Switch to the new scene immediately
		// newScene cannot be nullptr
		void ChangeScene(std::unique_ptr<Scene> newScene);

	private:
		// Called when the scene should update
		void Update();

		// Called when the scene should render
		void Render();

		// Called when the scene should render UI
		void RenderUI();

		// If m_NextScene is not nullptr, switch to it
		// Returns true if a scene change occurred
		// Returns false if no scene change occurred
		bool CheckForSceneChange();

	private:
		std::unique_ptr<Scene> m_CurrentScene;
		std::unique_ptr<Scene> m_NextScene;
		Chimp::Engine& m_Engine;
	};
}