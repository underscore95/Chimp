#include "EditorScene.h"
#include "Main.h"

int main(int argc, char** argv) {
	return Chimp::EntryPoint::RunChimp([](Chimp::Engine& engine) {
		ImGui::SetCurrentContext(engine.GetWindow().GetImGuiHandler().GetImGuiContext());
		return std::make_unique<ChimpEditor::EditorScene>(engine);
		}, argc, argv);
}