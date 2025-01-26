#pragma once

#include "stdafx.h"

namespace Chimp {
	// https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x

	inline void HashCombine(std::size_t& seed) { }

	template <typename T, typename... Rest>
	inline void HashCombine(std::size_t& seed, const T& v, Rest... rest) {
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		HashCombine(seed, rest...);
	}
}

// Makes a struct hashable by combining the hashes of its members
// Must be used in the global namespace
#define MAKE_HASHABLE(type, ...) \
    template<> struct std::hash<type> {\
        std::size_t operator()(const type &t) const {\
            std::size_t ret = 0;\
            Chimp::HashCombine(ret, __VA_ARGS__);\
            return ret;\
        }\
    };