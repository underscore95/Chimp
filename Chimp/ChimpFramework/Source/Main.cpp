#include "Main.h"
#include "MainLoop.h"
#include "api/Engine.h"

std::unique_ptr<Chimp::Engine> Chimp::EntryPoint::Engine = std::unique_ptr<Chimp::Engine>(new Chimp::Engine());

int main(int argc, char** argv)
{
	using namespace Chimp;
	std::unique_ptr<Scene> entryScene = CreateEntryScene(*EntryPoint::Engine);
	MainLoop mainLoop(std::move(entryScene), *EntryPoint::Engine);
    return 0;
}