#pragma once

#include "stdafx.h"

class Game : public Chimp::IGame {
public:
	void Setup(Chimp::Engine& engine) override;
	std::unique_ptr<Chimp::Scene> CreateRuntimeEntryScene(Chimp::Engine& engine) override;
};

CHIMP_SET_GAME(Game);