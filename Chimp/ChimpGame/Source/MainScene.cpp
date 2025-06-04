#include "MainScene.h"

MainScene::MainScene(Chimp::Engine& engine) :
	m_engine(engine),
	m_ecs(engine.CreateECS()),
	m_controller(m_camera, engine.GetWindow().GetInputManager())
{
	LoadResources();
}

MainScene::~MainScene()
{
	UnloadResources();
}

void MainScene::OnInit() {

	// Camera
	m_camera.SetPosition(Chimp::Vector3f{ 10, 6, 10 });
	m_camera.SetForwardVector(m_camera.GetPosition() * -1.0f); // Look at 0 0 0
}

void MainScene::OnActivate(std::unique_ptr<Scene> previousScene) {
	m_engine.GetWindow().SetTitle("Chimp Main");
	m_engine.GetWindow().SetSize({ 1280, 720 });
	m_engine.GetWindow().SetResizable(true);

	auto& shader = m_engine.GetRenderingManager().GetChimpShaders().GetLitShader();
	shader.SetCamera(m_camera);
}

void MainScene::OnDeactivate() {}

void MainScene::OnUpdate() {
	m_ecs->GetSystems().OnUpdate();
	m_ecs->GetTransformManager().UpdateAllMatrices();
}

void MainScene::OnRender() {
	auto& rm = m_engine.GetRenderingManager();
	auto& shader = m_engine.GetRenderingManager().GetChimpShaders().GetLitShader();
	shader.SetLighting(m_ecs->GetLightManager().GenerateLighting());

	m_ecs->GetSystems().OnRender();

	rm.SetViewport({ 0,0 }, m_engine.GetWindow().GetSize());
	rm.SetFrameBuffer();
	rm.GetRenderer().SetClearColor(0, 0, 0);
	rm.ClearColorBuffer();
}

void MainScene::OnRenderUI() {
	m_ecs->GetSystems().OnRenderUI();
}

void MainScene::LoadResources() {

}

void MainScene::UnloadResources() {

}