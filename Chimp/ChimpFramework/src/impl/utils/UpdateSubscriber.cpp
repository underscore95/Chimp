#pragma once

#include "api/utils/UpdateSubscriber.h"

namespace Chimp {
	void UpdateSubscriber::SubscribeUpdate(const std::function<void()> &callback)
	{
		m_UpdateSubscribers.push_back(callback);
	}

	void UpdateSubscriber::SubscribeRender(const std::function<void()>& callback)
	{
		m_RenderSubscribers.push_back(callback);
	}

	void UpdateSubscriber::SubscribeRenderUI(const std::function<void()>& callback)
	{
		m_RenderUISubscribers.push_back(callback);
	}

	void UpdateSubscriber::SubscribeShutdown(const std::function<void()>& callback)
	{
		m_ShutdownSubscribers.push_back(callback);
	}

	void UpdateSubscriber::Update()
	{
		for (auto& subscriber : m_UpdateSubscribers) {
			subscriber();
		}
	}

	void UpdateSubscriber::Render()
	{
		for (auto& subscriber : m_RenderSubscribers) {
			subscriber();
		}
	}

	void UpdateSubscriber::RenderUI()
	{
		for (auto& subscriber : m_RenderUISubscribers) {
			subscriber();
		}
	}

	void UpdateSubscriber::Shutdown()
	{
		for (auto& subscriber : m_ShutdownSubscribers) {
			subscriber();
		}
	}
}