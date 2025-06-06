#pragma once

#include "stdafx.h"

namespace Chimp {
	template <typename... Ts, size_t... Is>
	std::tuple<const Ts...> CopyTupleImpl(const std::tuple<Ts&...>& refTuple, std::index_sequence<Is...>) {
		return std::tuple<const Ts...>{ std::get<Is>(refTuple)... };
	}

	template <typename... Ts>
	std::tuple<const Ts...> CopyTuple(const std::tuple<Ts&...>& refTuple) {
		return CopyTupleImpl(refTuple, std::index_sequence_for<Ts...>{});
	}

}