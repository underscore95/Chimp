#pragma once

#include "TowerTypes.h"

struct TowerComponent {
	TowerType Type;
	Chimp::InPlaceOptional<Chimp::EntityId> Target;
	float AttackInterval = 1;
	float SecondsUntilNextAttack = 0;
	int AttackDamage = 1;
};