#pragma once

#include "stdafx.h"

namespace ChimpEditor {
	struct EntityNameComponent {
		std::string Name;
	};

	static Chimp::ComponentRegister<EntityNameComponent> Register;
}