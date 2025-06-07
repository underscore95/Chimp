#include "MainScene.h"

MainScene::MainScene(Chimp::Engine& engine) :
	m_engine(engine),
	m_controller(m_camera, engine.GetWindow().GetInputManager())
{
	m_ecs = engine.LoadECS(std::string(GAME_DATA_FOLDER) + "/ecs.json");

	LoadResources();
}

MainScene::~MainScene()
{
	UnloadResources();
}

void MainScene::OnInit() {

}

void MainScene::OnActivate(std::unique_ptr<Scene> previousScene) {
	m_engine.GetWindow().SetTitle("Chimp Main");
	m_engine.GetWindow().SetSize({ 1280, 720 });
	m_engine.GetWindow().SetResizable(true);

	// Camera
	m_camera.SetPosition(Chimp::Vector3f{ 10, 6, 10 });
	m_camera.SetForwardVector(m_camera.GetPosition() * -1.0f); // Look at 0 0 0

	auto& shader = m_engine.GetRenderingManager().GetChimpShaders().GetLitShader();
	shader.SetCamera(m_camera);
}

void MainScene::OnDeactivate() {}

void MainScene::OnUpdate() {
	m_ecs->Update();
}

void MainScene::OnRender() {
	auto& rm = m_engine.GetRenderingManager();

	rm.GetRenderer().SetClearColor(0.3f, 0.3f, 0.6f);
	rm.ClearColorBuffer();
	rm.ClearDepthBuffer();

	auto& shader = m_engine.GetRenderingManager().GetChimpShaders().GetLitShader();
	shader.SetLighting(m_ecs->GetLightManager().GenerateLighting());

	m_ecs->Render(shader);

	rm.SetDefaultRenderTarget(std::weak_ptr<Chimp::IRenderTexture>());
	rm.BindDefaultRenderTarget();
}

void MainScene::OnRenderUI() {
	m_ecs->RenderUI();
}

void MainScene::LoadResources() {

}

void MainScene::UnloadResources() {

}