#include "api/ecs/ECS.h"
#include "api/ecs/components/ComponentRegistry.h"

namespace Chimp {
	void ECS::GetComponentsOnEntity(EntityId entity, const std::function<void(AnyConstReference)>& function)
	{
		entity.each([this, entity, &function](flecs::id id) {
			auto it = m_ComponentIdToTypeInfo.find(id.raw_id());
			assert(it != m_ComponentIdToTypeInfo.end());
			TypeInfo typeInfo = it->second;
			void* componentPtr = entity.get_mut(id);
			assert(componentPtr);
			function(AnyConstReference{ typeInfo,componentPtr });
			});
	}
}