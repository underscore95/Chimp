#pragma once

#include "GameServer.h"

GameServer::GameServer(Chimp::Engine& engine)
	: m_Engine(engine)
{
	m_Engine.GetUpdateSubscriber().SubscribeUpdate([this]() {
		Update();
		});
}

void GameServer::Start(Chimp::ConnectionInfo connectionInfo)
{
	m_Server = m_Engine.HostServer(connectionInfo);
	if (!m_Server)
	{
		GetLogger().Error("Failed to start server");
	}
	else
	{
		m_Handlers = std::make_unique<ServerHandlers>();
		m_Handlers->MatchHandler = std::make_unique<MatchHandler>(m_Engine, *m_Server);
		m_Handlers->WaveHandler = std::make_unique<WaveHandler>(m_Engine, *m_Server, *m_Handlers->MatchHandler);
	}
}

void GameServer::Shutdown()
{
	if (m_Server)
	{
		assert(m_Handlers);
		m_Handlers.reset();
		m_Server.reset();
	}
}

void GameServer::Update()
{
	if (m_Server)
	{
		assert(m_Handlers);
		m_Server->Update();
		m_Handlers->MatchHandler->Update();
	}
}

Chimp::IServer& GameServer::GetServer()
{
	assert(m_Server);
	return *m_Server;
}

ServerHandlers& GameServer::GetHandlers()
{
	assert(m_Handlers);
	return *m_Handlers;
}
