#pragma once

#include "api/scenes/Scene.h"

namespace Chimp {
	class Engine;
	class EntryPoint {
	public:
		static int RunChimp(std::function<std::unique_ptr<Chimp::Scene>(Chimp::Engine&)> entrySceneCreator, int argc, char** argv);
	};
}