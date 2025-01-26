#pragma once

#include "stdafx.h"

struct ServerWaveEndPacket : public Chimp::NetworkPacket
{
	unsigned int WaveIndex;
	unsigned int MatchId;
};