#include "Main.h"
#include "MainLoop.h"
#include "api/Engine.h"
#include <stdlib.h>
#include "api/ecs/components/ComponentRegistry.h"

int Chimp::EntryPoint::RunChimp(std::function< std::unique_ptr<Chimp::Scene>(Chimp::Engine&)> entrySceneCreator, int argc, char** argv)
{
	system("CLS"); // TODO: do this in a nicer way

	using namespace Chimp;
	auto Engine = std::unique_ptr<Chimp::Engine>(new Chimp::Engine());
	{
		MainLoop mainLoop(entrySceneCreator(*Engine), *Engine);
	}
	Engine->Destroy(); // Required because OpenAL audio device can't be destroyed in global destructor

    return 0;
}