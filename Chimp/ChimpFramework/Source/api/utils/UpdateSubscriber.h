#pragma once

#include "stdafx.h"

// Get instance of this class from Engine
namespace Chimp {
	class Engine;
	class MainLoop;
	class UpdateSubscriber {
		friend class Engine;
		friend class MainLoop;
	private:
		UpdateSubscriber() = default;
	public:		
		// Subscribe to the main loop forever
		// The point at which the callback is called is undetermined, only that e.g update will be called during update phase is guaranteed.
		// callback - function to call when the update event is triggered
		void SubscribeUpdate(const std::function<void()>& callback);
		void SubscribeRender(const std::function<void()>& callback);
		void SubscribeRenderUI(const std::function<void()>& callback);
		void SubscribeShutdown(const std::function<void()>& callback);

	private:
		void Update();
		void Render();
		void RenderUI();
		void Shutdown();

	private:
		std::vector<std::function<void()>> m_UpdateSubscribers;
		std::vector<std::function<void()>> m_RenderSubscribers;
		std::vector<std::function<void()>> m_RenderUISubscribers;
		std::vector<std::function<void()>> m_ShutdownSubscribers;
	};
}