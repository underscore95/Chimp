#pragma once

namespace Chimp {
	struct HealthComponent {
		int Health = 1; // If reaches 0, entity dies (this is not built into chimp, make a system that does this yourself)
	};
}