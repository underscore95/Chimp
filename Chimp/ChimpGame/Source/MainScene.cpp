#include "MainScene.h"

MainScene::MainScene(Chimp::Engine& engine) :
	m_engine(engine),
	m_controller(m_camera, engine.GetWindow().GetInputManager())
{
	LoadEntities();

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

	rm.GetRenderer().SetClearColor(0.3f, 0.3f, 0.6f);
	rm.ClearColorBuffer();
	rm.ClearDepthBuffer();

	auto& shader = m_engine.GetRenderingManager().GetChimpShaders().GetLitShader();
	shader.SetLighting(m_ecs->GetLightManager().GenerateLighting());

	m_ecs->GetSystems().OnRender();

	auto view = m_ecs->GetEntitiesWithComponents<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>();
	shader.RenderWorld(view, *m_ecs);

	rm.SetDefaultRenderTarget(std::weak_ptr<Chimp::IRenderTexture>());
	rm.BindDefaultRenderTarget();
}

void MainScene::OnRenderUI() {
	m_ecs->GetSystems().OnRenderUI();
}

void MainScene::LoadResources() {

}

void MainScene::UnloadResources() {

}

void MainScene::LoadEntities()
{
	do {
		const std::string path = std::string(GAME_DATA_FOLDER) + "/ecs.json";

		if (!std::filesystem::exists(path))
			break;

		std::ifstream file(path);
		if (!file.is_open())
			break;

		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string jsonContent = buffer.str();

		m_ecs = Chimp::ECS::Deserialise(m_engine, jsonContent, true, true);
	} while (false);

	if (m_ecs == nullptr) m_ecs = m_engine.CreateECS();
}
