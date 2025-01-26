#include "GameClient.h"

GameClient::GameClient(Chimp::Engine& engine) : m_Engine(engine)
{
	m_Engine.GetUpdateSubscriber().SubscribeUpdate([this]() {
		Update();
		});
}

void GameClient::Connect(Chimp::ConnectionInfo connectionInfo)
{
	m_Client = m_Engine.ConnectToServer(connectionInfo);
	if (!m_Client)
	{
		GetLogger().Error("Failed to connect to server");
	}
	else {
		m_Handlers = std::make_unique<ClientHandlers>();
		m_Handlers->CurrentMatchHandler = std::make_unique<ClientCurrentMatchHandler>(m_Engine, *m_Client);
		m_Handlers->WinListener = std::make_unique<GameWinListener>(*m_Client);
		m_Handlers->WaveListener = std::make_unique<WaveStartListener>(*m_Client);
		m_Handlers->TowerListener = std::make_unique<TowerListener>(*m_Client);
		m_Handlers->BloonListener = std::make_unique<BloonListener>(*m_Client);
		GetLogger().Info("Connected to server with client id " + std::to_string(m_Client->GetId()));
	}
}

bool GameClient::IsConnected()
{
	if (m_Client && m_Client->WasDisconnected())
	{
		m_Handlers.reset();
		m_Client.reset();
	}
	return m_Client != nullptr;
}

void GameClient::Update()
{
	if (m_Client)
	{
		m_Client->Update();
		m_Handlers->CurrentMatchHandler->Update();
	}
}

Chimp::IClient& GameClient::GetClient()
{
	bool connected = IsConnected();
	assert(connected);
	return *m_Client;
}

ClientHandlers& GameClient::GetHandlers()
{
	assert(m_Handlers);
	return *m_Handlers;
}
