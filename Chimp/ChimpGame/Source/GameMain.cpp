#pragma message("Compiling GameMain.cpp")

#include "EntryScene.h"
#include "Main.h"
#include <iostream>

int main(int argc, char** argv) {
	return Chimp::EntryPoint::RunChimp([](Chimp::Engine& engine) { return std::make_unique<EntryScene>(engine); }, argc, argv);
}