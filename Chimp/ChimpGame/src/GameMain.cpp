#include "EntryScene.h"

std::unique_ptr<Chimp::Scene> CreateEntryScene(Chimp::Engine &engine) {
	return std::make_unique<EntryScene>(engine);
}