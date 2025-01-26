#pragma once

#include "stdafx.h"

struct ServerMatchEndPacket : public Chimp::NetworkPacket
{
	int MatchId;
};