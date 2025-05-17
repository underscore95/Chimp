#pragma once

#include "api/utils/Json.h"

#ifdef CHIMP_FLECS
#include <flecs.h>

namespace Chimp {
	typedef flecs::entity EntityId;

	// world can be nullptr but in that case you can't use it to add components or anything
	static EntityId SizeTToEntityId(size_t id, const flecs::world_t* world) {
		static_assert(sizeof(std::size_t) >= sizeof(flecs::entity_t));
		return flecs::entity(world, id);
	};

	inline void to_json(Json& j, const EntityId& e) {
		j = e.id();
	}

	// Warning! Output entity will not be associated with any world
	inline void from_json(const Json& j, EntityId& e) {
		std::size_t id = j.get<std::size_t>();
		e = SizeTToEntityId(id, nullptr);
	}
}

namespace std {
	template<>
	struct hash<::Chimp::EntityId>
	{
		std::size_t operator()(const Chimp::EntityId& id) const noexcept {
			std::uint64_t n= id.id();
			return std::hash<std::uint64_t>{}(n); // hash because its probably not good distribution, even though it is unique
		}
	};
}

#endif