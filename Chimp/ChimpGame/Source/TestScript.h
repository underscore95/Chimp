#pragma once

#include "stdafx.h"

class TestScript : public Chimp::IEntityScript {
public:
	TestScript(Chimp::Engine& engine, Chimp::ECS& ecs) : IEntityScript("Test", engine, ecs) {}

	void OnInit() {
		std::cout << "Initialised script on ecs " << &GetECS() << "\n";
	};
};