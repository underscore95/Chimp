#include "EditorScene.h"
#include "Main.h"
#include <iostream>

int main(int argc, char** argv) {
	return Chimp::EntryPoint::RunChimp([](Chimp::Engine& engine) { return std::make_unique<ChimpEditor::EditorScene>(engine); }, argc, argv);
}