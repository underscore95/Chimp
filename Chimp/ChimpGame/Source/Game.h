#pragma once

#include "stdafx.h"

class Game : public Chimp::IGame {
public:
	void Setup(Chimp::Engine& engine);
};
CHIMP_SET_GAME(Game);