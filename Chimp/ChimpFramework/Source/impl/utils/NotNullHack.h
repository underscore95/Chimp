#pragma once

#include "stdafx.h"

namespace Chimp {
	// Value is definitely not null, but the compiler doesn't know that, mainly used to get around having to initializer list (in some places it isn't practical)
	template <typename T>
	using NotNullHack = std::unique_ptr<T>;
}