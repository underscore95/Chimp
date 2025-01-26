#pragma once

#include "stdafx.h"

struct ClientMatchStartPacket : public Chimp::NetworkPacket
{
	int OpponentId;
	int MatchId;
	int MapFileIndex;
};