#pragma once

#include "stdafx.h"

namespace Chimp {
	template <typename T>
	struct Resource {
		Resource() : RefCount(0), Data(nullptr) {}
		Resource(std::unique_ptr<T> data) : RefCount(0), Data(std::move(data)) {}

		size_t RefCount;
		std::unique_ptr<T> Data;
	};
}