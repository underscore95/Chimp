#pragma once

#include "api/Engine.h"
#ifdef CHIMP_GLFW
#include "impl/window/WindowGLFW.h"
#endif

#ifdef CHIMP_OPENGL
#include "impl/graphics/opengl/RenderingManager.h"
#endif

#ifdef CHIMP_STB
#include "impl/graphics/images/stb/ImageLoader.h"
#endif

#ifdef CHIMP_ENET
#include "impl/networking/enet/ServerFactory.h"
#endif

#include "api/networking/PacketTypeRegistry.h"
#include "Loggers.h"
#include "api/audio/sfx/SoundEffect.h"

#include "api/ecs/components/ComponentRegistry.h"

namespace Chimp {

	static Reference<Engine> g_Engine;

	Engine::Engine() :
		m_ResourceManager(*this),
		m_Window(CreateWindow()),
		m_ImageLoader(CreateImageLoader()),
		m_RenderingManager(CreateRenderingManager()),
		m_ThreadPool(std::thread::hardware_concurrency() - 1),
		m_AudioManager(std::make_unique<AudioManager>()),
		m_MusicPlayer(*this)
	{
		assert(!g_Engine);
		g_Engine = this;
		m_ResourceManager.InitModelImporter();

		PacketTypeRegistry::RegisterChimpPacketTypes();
	}

	void Engine::PostInit()
	{
		m_RenderingManager->InitChimpShaders(*this);

		Loggers::Main().Info("Initialized Chimp Engine!");
	}

	void Engine::Destroy()
	{
		m_SceneManager = nullptr;

		// Can't destroy an OpenAL context in a thread destructor
		m_AudioManager = nullptr;
	}

	Engine::~Engine()
	{
		m_RenderingManager->DestroyChimpShaders();
		g_Engine = nullptr;
	}

	Engine& Engine::GetEngine()
	{
		assert(g_Engine);
		return g_Engine;
	}

	TimeManager& Engine::GetTimeManager()
	{
		return m_TimeManager;
	}

	IWindow& Engine::GetWindow()
	{
		return *m_Window;
	}

	IRenderingManager& Engine::GetRenderingManager()
	{
		return *m_RenderingManager;
	}

	ResourceManager& Engine::GetResourceManager()
	{
		return m_ResourceManager;
	}

	UpdateSubscriber& Engine::GetUpdateSubscriber()
	{
		return m_UpdateSubscriber;
	}

	SceneManager& Engine::GetSceneManager()
	{
		return *m_SceneManager;
	}

	ThreadPool& Engine::GetThreadPool()
	{
		return m_ThreadPool;
	}

	ImGuiHelper& Engine::GetImGuiHelper()
	{
		return m_ImGuiHelper;
	}

	Random& Engine::GetRandom()
	{
		return m_Random;
	}

	AudioManager& Engine::GetAudioManager()
	{
		return *m_AudioManager;
	}

	MusicPlayer& Engine::GetMusicPlayer()
	{
		return m_MusicPlayer;
	}

	std::unique_ptr<TaskScheduler> Engine::CreateTaskScheduler()
	{
		return std::make_unique<TaskScheduler>(*this);
	}

	std::unique_ptr<IServer> Engine::HostServer(const ConnectionInfo& serverInfo)
	{
		if (!serverInfo.IsValid()) {
			Loggers::Main().Error("Server info is invalid.");
			return nullptr;
		}

		std::unique_ptr<IServer> server = nullptr;
#ifdef CHIMP_ENET
		server = ENetServerFactory::CreateServer(serverInfo);
#endif
		if (server == nullptr) {
			Loggers::Main().Error("No networking implementation available.");
			return nullptr;
		}
		if (!server->IsValid()) {
			Loggers::Main().Error("Server is invalid, this means hosting failed.");
			return nullptr;
		}
		return std::move(server);
	}

	std::unique_ptr<IClient> Engine::ConnectToServer(const ConnectionInfo& serverInfo)
	{
		if (!serverInfo.IsValid()) {
			Loggers::Main().Error("Server info is invalid.");
			return nullptr;
		}

		std::unique_ptr<IClient> client = nullptr;
#ifdef CHIMP_ENET
		client = ENetServerFactory::CreateClient(serverInfo);
#endif
		if (client == nullptr) {
			Loggers::Main().Error("No networking implementation available.");
			return nullptr;
		}
		if (!client->IsValid()) {
			Loggers::Main().Error("Client is invalid, this means connecting failed.");
			return nullptr;
		}
		return std::move(client);
	}

	std::unique_ptr<Logger> Engine::CreateLogger(const std::string& name, LogLevel level)
	{
		return std::make_unique<Logger>(name, level);
	}

	std::unique_ptr<ECS> Engine::CreateECS()
	{
		auto ecs= std::unique_ptr<ECS>(new ECS(*this));
		ComponentRegistry::Instance().RegisterComponentsInECS(*ecs);
		return std::move(ecs);
	}

	std::unique_ptr<ECS> Engine::LoadECS(const std::string& filePath)
	{
		do {
			if (!std::filesystem::exists(filePath))
				break;

			std::ifstream file(filePath);
			if (!file.is_open())
				break;

			std::stringstream buffer;
			buffer << file.rdbuf();
			std::string jsonContent = buffer.str();

			return Chimp::ECS::Deserialise(*this, jsonContent, true, true);
		} while (false);

		return CreateECS();
	}

	std::unique_ptr<IWindow> Engine::CreateWindow() const
	{
		std::unique_ptr<IWindow> window = nullptr;
#ifdef CHIMP_GLFW
		window = std::make_unique<WindowGLFW>();
#endif
		if (window == nullptr) {
			Loggers::Main().Error("No window implementation available.");
			return nullptr;
		}
		window->GetImGuiHandler().Init(*window);
		return std::move(window);
	}

	std::unique_ptr<IRenderingManager> Engine::CreateRenderingManager()
	{
		assert(m_ImageLoader);
		std::unique_ptr<IRenderingManager> renderingManager = nullptr;
#ifdef CHIMP_OPENGL
		renderingManager = std::make_unique<GL::RenderingManager>(this, *m_ImageLoader);
#endif

		if (!renderingManager) {
			Loggers::Main().Error("No rendering manager implementation available.");
			return nullptr;
		}

		renderingManager->GetRenderer().SetClearColor(0.0f, 0.0f, 0.0f);
		return renderingManager;
	}

	std::unique_ptr<IImageLoader> Engine::CreateImageLoader() const
	{
#ifdef CHIMP_STB
		return std::make_unique<STB::ImageLoader>();
#endif
		Loggers::Main().Error("No image loader implementation available.");
		return nullptr;
	}
}