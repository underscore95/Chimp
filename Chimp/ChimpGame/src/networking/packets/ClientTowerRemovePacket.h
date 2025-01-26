#pragma once

#include "game/Entities.h"
#include "stdafx.h"

struct ClientTowerRemovePacket : public Chimp::NetworkPacket {
	NetworkId TowerId;
};