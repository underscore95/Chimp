#include "EditorScene.h"
#include "Main.h"

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
		return std::make_unique<ChimpEditor::EditorScene>(engine);
		}, argc, argv);
}