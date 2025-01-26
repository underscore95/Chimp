#include "WaveHandler.h"
#include "networking/Networking.h"

WaveHandler::WaveHandler(Chimp::Engine& engine, 
	Chimp::IServer& server,
	MatchHandler& matchHandler) :
	m_Engine(engine),
	m_Server(server),
	m_MatchHandler(matchHandler)
{
	m_WaveEndListener = m_Server.GetEventHandler().Subscribe(
		Networking::SERVER_WAVE_END,
		[this](const Chimp::NetworkPacket* event) { HandleReceivedWaveEndPacket(event); }
	);

	matchHandler.AddMatchEndCallback([this](const ServerMatch& match) { HandleMatchEnd(match); });
}

WaveHandler::~WaveHandler()
{
	// Technically leaking memory since we don't remove the match handler lambda but its fine because
	// both are instantiated and destructed at the same time
	m_Server.GetEventHandler().Unsubscribe(m_WaveEndListener);
}

void WaveHandler::HandleReceivedWaveEndPacket(const Chimp::NetworkPacket* event)
{
	auto packet = static_cast<const ServerWaveEndPacket*>(event);
	auto matchId = packet->MatchId;

	auto iter = m_ReceivedWaveEndPackets.find(matchId);
	if (iter == m_ReceivedWaveEndPackets.end())
	{
		m_ReceivedWaveEndPackets.emplace(matchId, packet->WaveIndex);
		return;
	}
	else {
		StartWave(matchId, packet->WaveIndex + 1);
	}
}

void WaveHandler::HandleMatchEnd(const ServerMatch& match)
{
	m_ReceivedWaveEndPackets.erase(match.GetMatchId());
}

void WaveHandler::StartWave(int matchId, int nextWaveIndex)
{
	m_ReceivedWaveEndPackets.erase(matchId);

	// Send start packet
	ClientStartWavePacket packet = {};
	packet.PacketType = Networking::CLIENT_START_WAVE;
	packet.WaveIndex = nextWaveIndex;
	packet.MatchId = matchId;

	auto [player1, player2] = m_MatchHandler.GetPlayersInMatch(matchId);
	m_Server.SendPacketToClient(player1, packet);
	m_Server.SendPacketToClient(player2, packet);
}
