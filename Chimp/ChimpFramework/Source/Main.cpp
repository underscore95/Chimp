#include "Main.h"
#include "MainLoop.h"
#include "api/Engine.h"
#include <stdlib.h>

std::unique_ptr<Chimp::Engine> Chimp::EntryPoint::Engine = nullptr;

int Chimp::EntryPoint::RunChimp(std::function< std::unique_ptr<Chimp::Scene>(Chimp::Engine&)> entrySceneCreator, int argc, char** argv)
{
	system("CLS"); // TODO: do this in a nicer way

	using namespace Chimp;
	Engine = std::unique_ptr<Chimp::Engine>(new Chimp::Engine());
	MainLoop mainLoop(entrySceneCreator(*Engine), *Engine);
    return 0;
}