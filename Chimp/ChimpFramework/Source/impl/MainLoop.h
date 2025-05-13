#pragma once

#include "api/scenes/SceneManager.h"

namespace Chimp {
	class Engine;
	class MainLoop {
	public:
		MainLoop(std::unique_ptr<Scene> entryScene, Engine& engine);

	private:
		Engine& m_Engine;
	};
}