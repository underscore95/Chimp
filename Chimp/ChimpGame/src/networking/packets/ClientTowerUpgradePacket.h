#pragma once

#include "game/Entities.h"
#include "stdafx.h"

struct ClientTowerUpgradePacket : public Chimp::NetworkPacket {
	NetworkId TowerId;
	UpgradeType UpgradeType;
};