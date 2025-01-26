#pragma once

#include "stdafx.h"
#include "BaseEntity.h"
#include "bloons/MoveableComponent.h"
#include "bloons/BloonComponent.h"

namespace Bloons {
	constexpr size_t NUM_BLOON_TYPES = 8;
	enum class BloonType {
		RED = 0, 
		BLUE = 1,
		GREEN = 2,
		YELLOW = 3,
		PINK = 4,
		BLACK = 5,
		ZEBRA = 6,
		RAINBOW = 7
	};
	const std::array<std::string, NUM_BLOON_TYPES> TexturePaths = {
		"Bloons/Red.png",
		"Bloons/Blue.png",
		"Bloons/Green.png",
		"Bloons/Yellow.png",
		"Bloons/Pink.png",
		"Bloons/Black.png",
		"Bloons/Zebra.png",
		"Bloons/Rainbow.png"
	};
	const std::array<int, NUM_BLOON_TYPES> HealthValues = {
		1,
		2,
		3,
		4,
		5,
		6,
		7,
		8
	};
	const std::array<float, NUM_BLOON_TYPES> SpeedValues = {
		75.0f,
		100.0f,
		150.0f,
		200.0f,
		300.0f,
		100.0f,
		200.0f,
		500.0f
	};
	constexpr BloonType HealthToBloonType(int health) {
		if (health <= 1) {
			return BloonType::RED;
		}
		else if (health == 2) {
			return BloonType::BLUE;
		}
		else if (health == 3) {
			return BloonType::GREEN;
		}
		else if (health == 4) {
			return BloonType::YELLOW;
		}
		else if (health == 5) {
			return BloonType::PINK;
		}
		else if (health == 6) {
			return BloonType::BLACK;
		}
		else if (health == 7) {
			return BloonType::ZEBRA;
		}
		else {
			return BloonType::RAINBOW;
		}
	}

	void DamageBloon(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::EntityId bloonEntity, int damage);
}

namespace Entities {
	Chimp::EntityId CreateBloonEntity(Chimp::ECS& ecs, Chimp::Engine& engine, Chimp::Vector2f position, Bloons::BloonType type);
}