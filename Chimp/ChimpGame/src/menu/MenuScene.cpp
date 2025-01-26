#include "MenuScene.h"
#include "game/GameScene.h"
#include "Debug.h"
#include "Logger.h"

MenuScene::MenuScene(Chimp::Engine& engine,
	std::shared_ptr<Chimp::GameShader> shader)
	: m_Engine(engine),
	m_ConnectionInfo({}),
	m_MapList(Chimp::YAMLBlockParser::Parse(GAME_DATA_FOLDER + std::string("/Maps/Maps.yml")).Data),
	m_Settings(engine)
{
	LoadResources();

	m_Engine.GetWindow().SetTitle("Chimp Challenge");
	m_Engine.GetWindow().SetSize({ 1280, 720 });
	m_Engine.GetWindow().SetResizable(true);

	m_Engine.GetAudioManager().GetListener().SetPosition((Chimp::Vector3f)m_Engine.GetWindow().GetSize() / 2.0f);

	auto& renderingManager = m_Engine.GetRenderingManager();

	if (shader == nullptr) {
		m_GameShader = std::make_shared<Chimp::GameShader>(m_Engine, Chimp::ShaderFilePaths{
			GAME_DATA_FOLDER + std::string("/Assets/Shaders/Default.vert"),
			GAME_DATA_FOLDER + std::string("/Assets/Shaders/Default.frag")
			});

#ifdef DEBUG_AUTOHOST_AUTOCONNECT
		Networking::GetServer()->Start(m_ConnectionInfo);
		Networking::GetServer()->GetHandlers().MatchHandler->MapCount = m_MapList.StringArrays["Files"].size();
		Networking::GetClient()->Connect(m_ConnectionInfo);
#endif
	}
	else {
		m_GameShader = shader;
		Networking::GetServer()->Shutdown();
		Networking::GetClient()->Disconnect();
	}
}

MenuScene::~MenuScene()
{
	UnloadResources();
}

void MenuScene::OnInit()
{
}

void MenuScene::OnActivate(std::unique_ptr<Chimp::Scene> previousScene)
{
	m_Engine.GetMusicPlayer().SwitchMusic(Chimp::MusicTracksContainer{ {
		GAME_DATA_FOLDER + std::string("/Assets/Music/menu.wav")
		} });
}

void MenuScene::OnDeactivate()
{
}

void MenuScene::OnUpdate()
{
	if (!Networking::GetClient()->IsConnected()) return;
	auto& clientHandlers = Networking::GetClient()->GetHandlers();

	if (clientHandlers.CurrentMatchHandler->IsInMatch()) {
		std::string map = m_MapList.StringArrays["Files"][clientHandlers.CurrentMatchHandler->GetMapFileIndex()];
		m_Engine.GetSceneManager().QueueSceneChange(std::make_unique<GameScene>(m_Engine, m_GameShader, map));
	}
}

void MenuScene::OnRender()
{

}

void MenuScene::OnRenderUI()
{
	ImGui::SetNextWindowPos({ 100, 100 });
	ImGui::Begin("Networking", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	// Host
	static char buffer[256] = "";
	strcpy(buffer, m_ConnectionInfo.HostName.c_str());
	if (ImGui::InputText("Host", buffer, 256)) {
		m_ConnectionInfo.HostName = buffer;
	}

	// Port
	int port = m_ConnectionInfo.Port;
	ImGui::InputInt("Port", &port);
	if (port > 0 && port < 65535) {
		m_ConnectionInfo.Port = port;
	}

	// Timeout
	int timeout = m_ConnectionInfo.ConnectionTimeoutMs;
	ImGui::InputInt("Timeout", &timeout);
	if (timeout > 0) {
		m_ConnectionInfo.ConnectionTimeoutMs = timeout;
	}

	// Host
	bool disableHostButton = Networking::GetServer()->IsRunning();
	if (m_ConnectionInfo.HostName != "localhost") {
		ImGui::Text("You can only host on 'localhost' (you can connect on other ips)");
		disableHostButton = true;
	}
	ImGui::BeginDisabled(disableHostButton);
	if (ImGui::Button("Host Server")) {
		Networking::GetServer()->Start(m_ConnectionInfo);
		Networking::GetServer()->GetHandlers().MatchHandler->MapCount = m_MapList.StringArrays["Files"].size();
	}
	ImGui::EndDisabled();

	// Shutdown server
	ImGui::BeginDisabled(!Networking::GetServer()->IsRunning());
	ImGui::SameLine();
	if (ImGui::Button("Shutdown Server")) {
		Networking::GetServer()->Shutdown();
	}
	ImGui::EndDisabled();

	// Connect
	ImGui::BeginDisabled(Networking::GetClient()->IsConnected());
	if (ImGui::Button("Connect")) {
		Networking::GetClient()->Connect(m_ConnectionInfo);
	}
	ImGui::EndDisabled();

	// Disconnect
	ImGui::BeginDisabled(!Networking::GetClient()->IsConnected());
	ImGui::SameLine();
	if (ImGui::Button("Disconnect")) {
		Networking::GetClient()->Disconnect();
	}
	ImGui::EndDisabled();

	// Text
	if (Networking::GetServer()->IsRunning()) {
		ImGui::Text("Server is being hosted");
	}
	else {
		ImGui::Text("Server is not being hosted");
	}
	if (Networking::GetClient()->IsConnected()) {
		auto str = "Client is connected with id " + std::to_string(Networking::GetClient()->GetClient().GetId());
		ImGui::Text(str.c_str());
	}
	else {
		ImGui::Text("Client is not connected");
	}

	ImGui::End();

	ImGui::SetNextWindowPos({500, 100});
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	m_Settings.RenderUI();

	ImGui::End();
}

void MenuScene::LoadResources()
{
}

void MenuScene::UnloadResources()
{
}
