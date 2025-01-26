#include "ClientCurrentMatchHandler.h"
#include "networking/packets/Packets.h"
#include "networking/Networking.h"
#include "Logger.h"
#include "Debug.h"

ClientCurrentMatchHandler::ClientCurrentMatchHandler(Chimp::Engine& engine,
	Chimp::IClient& client) :
	m_Engine(engine),
	m_Client(client)
{
	m_MatchStartListener = m_Client.GetEventHandler().Subscribe(Networking::CLIENT_MATCH_START,
		[this](const Chimp::NetworkPacket* event) { HandleMatchStart(event); });

#ifdef DEBUG_AUTOSTART_WITH_1_PLAYER
	ClientMatchStartPacket matchStartPacket;
	matchStartPacket.PacketType = Networking::CLIENT_MATCH_START;
	matchStartPacket.MatchId = 1;
	matchStartPacket.OpponentId = 2;
	matchStartPacket.MapFileIndex = 1;
	HandleMatchStart(&matchStartPacket); 
#endif
}

ClientCurrentMatchHandler::~ClientCurrentMatchHandler()
{
	m_Client.GetEventHandler().Unsubscribe(m_MatchStartListener);
}

void ClientCurrentMatchHandler::Update()
{
}

void ClientCurrentMatchHandler::HandleMatchStart(const Chimp::NetworkPacket* event)
{
	auto matchStartPacket =
		static_cast<const ClientMatchStartPacket*>(event);
	assert(matchStartPacket != nullptr);

	if (IsInMatch()) {
		GetLogger().Error("Already in a match, dropping match start packet");
		return;
	}

	m_MatchId = matchStartPacket->MatchId;
	m_OpponentId = matchStartPacket->OpponentId;
	m_MapFileIndex = matchStartPacket->MapFileIndex;

	GetLogger().Info("Match started with opponent " + std::to_string(m_OpponentId));
}
