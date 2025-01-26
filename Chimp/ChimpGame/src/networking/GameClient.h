#pragma once

#include "stdafx.h"
#include "client/ClientCurrentMatchHandler.h"

#include "client/GameWinListener.h"
#include "client/WaveStartListener.h"
#include "client/TowerListener.h"
#include "client/BloonListener.h"

struct ClientHandlers {
	// not null ptrs (instantiated in Connect)
	std::unique_ptr<ClientCurrentMatchHandler> CurrentMatchHandler;
	std::unique_ptr<GameWinListener> WinListener; 
	std::unique_ptr<WaveStartListener> WaveListener;
	std::unique_ptr<TowerListener> TowerListener;
	std::unique_ptr<BloonListener> BloonListener;
};

class GameClient
{
public:
	GameClient(Chimp::Engine& engine);

	void Connect(Chimp::ConnectionInfo connectionInfo);

	bool IsConnected();
	void Disconnect() { 
		m_Handlers.reset();
		m_Client.reset(); 
	}

	Chimp::IClient& GetClient();
	ClientHandlers& GetHandlers();

private:
	void Update();

private:
	Chimp::Engine& m_Engine;
	std::shared_ptr<Chimp::IClient> m_Client;
	std::unique_ptr<ClientHandlers> m_Handlers;
};