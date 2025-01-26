#pragma once

#include "api/time/TimeManager.h"
#include "api/window/IWindow.h"
#include "api/graphics/IRenderingManager.h"
#include "api/graphics/images/IImageLoader.h"
#include "api/resources/ResourceManager.h"
#include "api/networking/EventHandler.h"
#include "api/networking/IServer.h"
#include "api/networking/IClient.h"
#include "api/logging/Logger.h"
#include "api/utils/UpdateSubscriber.h"
#include "scenes/SceneManager.h"
#include "api/threading/ThreadPool.h"
#include "api/utils/TaskScheduler.h"
#include "api/utils/ImGuiHelper.h"
#include "api/files/yaml/YAMLSerialiser.h"
#include "api/utils/Random.h"
#include "api/audio/AudioManager.h"
#include "api/audio/music/MusicPlayer.h"

namespace Chimp {
	class EntryPoint;
	class MainLoop;
	class Engine {
		friend class EntryPoint;
		friend class MainLoop;
	private:
		Engine();

	public:
		[[nodiscard]] TimeManager& GetTimeManager();
		[[nodiscard]] IWindow& GetWindow();
		[[nodiscard]] IRenderingManager& GetRenderingManager();
		[[nodiscard]] ResourceManager& GetResourceManager();
		[[nodiscard]] UpdateSubscriber& GetUpdateSubscriber();
		[[nodiscard]] SceneManager& GetSceneManager();
		[[nodiscard]] ThreadPool& GetThreadPool();
		[[nodiscard]] ImGuiHelper& GetImGuiHelper();
		[[nodiscard]] YAMLSerialiser& GetYAMLSerialiser();
		[[nodiscard]] Random& GetRandom();
		[[nodiscard]] AudioManager& GetAudioManager();
		[[nodiscard]] MusicPlayer& GetMusicPlayer();

		// Task scheduler for running tasks at specific times
		// You must update it once per frame
		[[nodiscard]] std::unique_ptr<TaskScheduler> CreateTaskScheduler();

		[[nodiscard]] std::unique_ptr<IServer> HostServer(const ConnectionInfo& serverInfo);
		[[nodiscard]] std::unique_ptr<IClient> ConnectToServer(const ConnectionInfo& serverInfo);

		// Create an enabled logger that logs to console and file
		// name - name of the logger, can be anything that is a valid directory name
		// level - minimum log level to log
		[[nodiscard]] std::unique_ptr<Logger> CreateLogger(const std::string& name, LogLevel level = LogLevel::INFO);

		// Create an event handler that can be used to send and receive events
		// EventType - enum representing event type
		// Event - struct representing event data, inheritance is supported
		template <typename EventType, typename Event>
		[[nodiscard]] EventHandlerAndBroadcaster<EventType, Event> CreateEventHandler() {
			return EventHandlerAndBroadcaster<EventType, Event>();
		}

	private:
		[[nodiscard]] std::unique_ptr<IWindow> CreateWindow() const;
		[[nodiscard]] std::unique_ptr<IRenderingManager> CreateRenderingManager() const;
		[[nodiscard]] std::unique_ptr<IImageLoader> CreateImageLoader() const;

	private:
		std::unique_ptr<AudioManager> m_AudioManager;
		std::unique_ptr<IWindow> m_Window; // Must be above rendering manager
		std::unique_ptr<IImageLoader> m_ImageLoader; // must be above rendering manager
		std::unique_ptr<IRenderingManager> m_RenderingManager;
		MusicPlayer m_MusicPlayer;
		ResourceManager m_ResourceManager; // Must be below stuff that this uses (e.g Audio, Rendering)
		TimeManager m_TimeManager;
		UpdateSubscriber m_UpdateSubscriber;
		std::unique_ptr<SceneManager> m_SceneManager; // initialized in MainLoop
		ThreadPool m_ThreadPool;
		ImGuiHelper m_ImGuiHelper;
		YAMLSerialiser m_YAMLSerialiser;
		Random m_Random;
	};
}