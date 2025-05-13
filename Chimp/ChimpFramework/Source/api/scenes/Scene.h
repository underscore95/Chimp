#pragma once

#include "stdafx.h"

namespace Chimp {
	class SceneManager;
	class Engine;

	class Scene {
		friend class SceneManager;
	public:
		Scene() = default;
		virtual ~Scene() = default;

	public:
		virtual bool ShouldExit(Chimp::Engine& engine) const;

	protected:
		// Called be the engine once after the scene is first switched to. Resources will be loaded before this is called.
		virtual void OnInit() = 0;

		// Called by the engine when the scene is activated, this may happen more than once
		// OnDeactivate must be called by the engine between calls to OnActivate
		// previousScene is the scene that was previously active, store for shop or pause scenes where the previous scenes data must be preserved.
		virtual void OnActivate(std::unique_ptr<Scene> previousScene = nullptr) = 0;

		// Called by the engine when the scene is deactivated, this may happen more than once
		virtual void OnDeactivate() = 0;

		// Called by the engine when the scene should update
		virtual void OnUpdate() = 0;

		// Called by the engine when the scene should render
		virtual void OnRender() = 0;

		// Called by the engine when the scene should render UI
		virtual void OnRenderUI() = 0;

		// Should depend on any resources that are required for the scene, never called by the engine.
		virtual void LoadResources() = 0;

		// Should release any resources that are no longer required, never called by the engine.
		virtual void UnloadResources() = 0;

	private:
		bool m_HasInitialized = false;
	};
}