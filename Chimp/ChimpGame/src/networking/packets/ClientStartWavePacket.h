#pragma once

#include "stdafx.h"

struct ClientStartWavePacket : public Chimp::NetworkPacket {
	unsigned int WaveIndex;
	unsigned int MatchId;
};