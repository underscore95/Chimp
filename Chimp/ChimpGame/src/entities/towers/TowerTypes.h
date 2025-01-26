#pragma once

static constexpr float TOWER_Z = -50;

typedef int TowerType;
static constexpr TowerType TOWER_TYPE_DART_MONKEY = 0;
static constexpr int NUM_TOWERS = 1;

static const std::array<std::string, NUM_TOWERS> TOWER_ICONS = {
	GAME_DATA_FOLDER + std::string("/Assets/Textures/Towers/Monkey.png")
};
static const std::array<int, NUM_TOWERS> TOWER_COSTS = {
	100
};