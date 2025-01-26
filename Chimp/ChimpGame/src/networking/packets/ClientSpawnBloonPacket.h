#pragma once

#include "game/Entities.h"
#include "stdafx.h"

struct ClientSpawnBloonPacket : public Chimp::NetworkPacket {
	Bloons::BloonType BloonType;
};