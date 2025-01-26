#pragma once

#include "stdafx.h"

struct MoveableComponent {
	MoveableComponent(float speed = 100) : Speed(speed), DistanceTravelled(0) {}

	float Speed;
	float DistanceTravelled;
};