#pragma once

#include "api/utils/Json.h"

#ifdef CHIMP_FLECS
#include <flecs.h>

namespace Chimp {
	typedef size_t EntityId;
	typedef flecs::entity Entity;

	// world can be nullptr but in that case you can't use it to add components or anything
	static Entity IdToEntity(size_t id, const flecs::world_t* world) {
		static_assert(sizeof(std::size_t) >= sizeof(flecs::entity_t));
		return flecs::entity(world, id);
	};

	inline void to_json(Json& j, const EntityId& e) {
		j = e;
	}

	// Warning! Output entity will not be associated with any world
	inline void from_json(const Json& j, EntityId& e) {
		e = j.get<std::size_t>();
	}
}

namespace std {
	template<>
	struct hash<::Chimp::Entity>
	{
		std::size_t operator()(const Chimp::Entity& e) const noexcept {
			return std::hash<Chimp::EntityId>{}(e.id());
		}
	};
}

#endif