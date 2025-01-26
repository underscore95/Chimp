#include "menu/MenuScene.h"

std::unique_ptr<Chimp::Scene> CreateEntryScene(Chimp::Engine &engine) {
	Networking::Init(engine);
	return std::make_unique<MenuScene>(engine);
}