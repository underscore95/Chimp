#pragma once

#include "stdafx.h"
#include "TowerComponent.h"
#include "WorthComponent.h"
#include "UpgradableComponent.h"
#include "entities/NetworkedIdentifierComponent.h"

struct MonkeyTowerComponent {
	bool Padding;
};

namespace Entities {
	Chimp::EntityId CreateMonkeyTower(Chimp::ECS& ecs, Chimp::Mesh& mesh, Chimp::Vector2f position);
}