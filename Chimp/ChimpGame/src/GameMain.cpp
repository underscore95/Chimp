#include "menu/MenuScene.h"

std::unique_ptr<Chimp::Scene> CreateEntryScene(Chimp::Engine &engine) {
	return std::make_unique<MenuScene>(engine);
}