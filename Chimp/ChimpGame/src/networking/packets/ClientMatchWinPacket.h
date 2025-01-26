#pragma once

#include "stdafx.h"

struct ClientMatchWinPacket : public Chimp::NetworkPacket
{
	int MatchId;
};