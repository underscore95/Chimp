#pragma once

#include "game/Entities.h"
#include "stdafx.h"

struct ClientTowerPlacePacket : public Chimp::NetworkPacket {
	Chimp::Vector2f Position;
	TowerType Type;
	NetworkId TowerId;
};