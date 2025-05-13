#include "api/scenes/Scene.h"
#include "api/Engine.h"

namespace Chimp {
	bool Scene::ShouldExit(Chimp::Engine& engine) const
	{
		return engine.GetWindow().GetStatus() == Chimp::WindowStatus::SHOULD_CLOSE;
	}
}