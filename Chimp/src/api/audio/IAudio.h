#pragma once

#include "stdafx.h"

namespace Chimp {
	// Handles audio library setup
	// e.g creating device, context, etc

	class IAudio {
	protected:
		IAudio() = default;
	public:
		virtual ~IAudio() = default;

		[[nodiscard]] virtual bool IsValid() const = 0;
	};
}