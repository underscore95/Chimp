#include "EditorScene.h"

namespace ChimpEditor {
	EditorScene::EditorScene(Chimp::Engine& engine) :
		m_engine(engine)
	{
		GetLogger().Info("Editor launched.");
	}

	void EditorScene::OnInit() {}

	void EditorScene::OnActivate(std::unique_ptr<Scene> previousScene) {}

	void EditorScene::OnDeactivate() {}

	void EditorScene::OnUpdate() {}

	void EditorScene::OnRender() {}

	void EditorScene::OnRenderUI() {}

	void EditorScene::LoadResources() {}

	void EditorScene::UnloadResources() {}
}