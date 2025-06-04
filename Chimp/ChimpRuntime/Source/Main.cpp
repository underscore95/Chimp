#include "Main.h"
#include "Game.h" // This is necessary or IGame::Instance won't be set

int main(int argc, char** argv) {
	return Chimp::EntryPoint::RunChimp([](Chimp::Engine& engine) {
		namespace fs = std::filesystem;

		fs::path currentPath = fs::current_path();

		for (fs::path p = currentPath; !p.empty(); p = p.parent_path()) {
			if (p.filename() == "Data") {
				throw std::runtime_error("Parent directory named 'Data' is not allowed: " + p.string());
			}

			if (p == p.parent_path()) break;
		}

		CHIMP_SET_IMGUI_CONTEXT_IN_DOMAIN();

		Chimp::IGame::Instance().Setup(engine);

		return Chimp::IGame::Instance().CreateRuntimeEntryScene(engine);
		}, argc, argv);
}