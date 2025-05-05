#pragma once

#include "api/scenes/Scene.h"

namespace Chimp {
	class Engine;
	class EntryPoint {
	public:
		static std::unique_ptr<Engine> Engine;
	};
}

int main(int argc, char** argv);

extern std::unique_ptr<Chimp::Scene> CreateEntryScene(Chimp::Engine &engine);