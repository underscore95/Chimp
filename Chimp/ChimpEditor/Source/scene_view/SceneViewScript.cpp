#include "SceneViewScript.h"

namespace ChimpEditor {
	SceneViewScript::SceneViewScript(Chimp::EntityId entity, Chimp::Engine& engine, Chimp::ECS& ecs)
		:
		IEntityScript(entity, engine, ecs),
		m_sceneViewTexture(engine.GetRenderingManager().CreateRenderTexture(1920, 1080))
	{
	}

	void SceneViewScript::OnInit()
	{
		GetEngine().GetRenderingManager().SetDefaultRenderTarget(m_sceneViewTexture);
	}

	void SceneViewScript::OnUpdate()
	{
	}

	void SceneViewScript::OnRender()
	{
		auto& rm = GetEngine().GetRenderingManager();
		rm.BindDefaultRenderTarget();
		rm.GetRenderer().SetClearColor(0.3f, 0.3f, 0.6f);
		rm.ClearColorBuffer();
		rm.ClearDepthBuffer();
		// TODO: clear stencil buffer
	}

	void SceneViewScript::OnRenderUI()
	{
		ImVec2 sceneViewSize = GetEngine().GetWindow().GetSize() * 0.5f;
		ImGui::SetNextWindowPos(sceneViewSize, 0, { 0.5f,0.5f });
		ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Image(m_sceneViewTexture->GetImGuiImageHandle(), sceneViewSize);
		ImGui::End();
	}
}