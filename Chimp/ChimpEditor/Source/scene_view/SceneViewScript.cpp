#include "SceneViewScript.h"

namespace ChimpEditor {
	SceneViewScript::SceneViewScript(Chimp::Engine& engine, Chimp::ECS& ecs, Chimp::ECS& gameEcs)
		:
		IEntityScript(typeid(this).name(), engine, ecs),
		m_sceneViewTexture(engine.GetRenderingManager().CreateRenderTexture(1920, 1080)),
		m_gameEcs(gameEcs)
	{
	}

	void SceneViewScript::OnInit()
	{
	}

	void SceneViewScript::OnUpdate()
	{
	}

	void SceneViewScript::OnRender()
	{
		auto& rm = GetEngine().GetRenderingManager();
		rm.BeginDebugGroup("Scene View Render");
		rm.SetDefaultRenderTarget(m_sceneViewTexture);
		rm.BindDefaultRenderTarget();
		rm.GetRenderer().SetClearColor(0.3f, 0.3f, 0.6f);
		rm.ClearColorBuffer();
		rm.ClearDepthBuffer();
		// TODO: clear stencil buffer

		auto& shader = GetEngine().GetRenderingManager().GetChimpShaders().GetLitShader();
		shader.RenderWorld(m_gameEcs);

		rm.SetDefaultRenderTarget(std::weak_ptr<Chimp::IRenderTexture>());
		rm.BindDefaultRenderTarget();
		rm.EndDebugGroup();
	}

	void SceneViewScript::OnRenderUI()
	{
		CHIMP_SET_IMGUI_CONTEXT_IN_DOMAIN();
		assert(ImGui::GetCurrentContext());
		ImVec2 sceneViewSize = GetEngine().GetWindow().GetSize() * 0.5f;
		ImGui::SetNextWindowPos(sceneViewSize, 0, { 0.5f,0.5f });
		ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		
		ImGui::Image(m_sceneViewTexture->GetImGuiImageHandle(), sceneViewSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
}