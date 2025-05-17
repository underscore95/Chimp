#include "api/ecs/hierarchy/EntityHierarchy.h"
#include "api/ecs/hierarchy/HierarchyComponent.h"
#include "api/ecs/ECS.h"
#include "api/ecs/components/EntityIdComponent.h"

namespace Chimp {
	EntityHierarchy::EntityHierarchy(ECS& ecs) : m_ECS(ecs)
	{
	}

	void Chimp::EntityHierarchy::SetParent(EntityId child, EntityId parent)
	{
		if (child == parent) {
			Loggers::Main().Error(std::format("Attempted to make {} a parent of itself.", child));
			assert(false);
			return;
		}

		// Check if we have a parent
		auto hierarchyComp = m_ECS.GetMutableComponent<HierarchyComponent>(child);
		assert(hierarchyComp);
		if (hierarchyComp->HierarchyLevel > 0) {
			// TODO: Do nothing if the parent is the same?
			// We have a parent, let them know their child is leaving if they track children
			auto oldParentsComp = m_ECS.GetMutableComponent<HierarchyComponent>(hierarchyComp->Parent);
			assert(oldParentsComp);
			auto& children = oldParentsComp->Children;
			children.Remove(child);
		}

		// Set the new parent
		hierarchyComp->Parent = parent;

		auto parentComp = m_ECS.GetMutableComponent<HierarchyComponent>(parent);
		assert(parentComp);
		auto& children = parentComp->Children;
		children.Insert(child);

		hierarchyComp->HierarchyLevel = parentComp->HierarchyLevel + 1;
	}

	bool EntityHierarchy::IsChildOf(EntityId parent, EntityId possibleChild) const
	{
		auto trackChildrenComp = m_ECS.GetMutableComponent<HierarchyComponent>(parent);
		assert(trackChildrenComp);
		auto& children = trackChildrenComp->Children;
		return children.Contains(possibleChild);
	}

	void EntityHierarchy::OrphanChild(EntityId child) const
	{
		auto childComp = m_ECS.GetMutableComponent<HierarchyComponent>(child);
		assert(childComp);
		if (childComp->HierarchyLevel <= 0) return;
		auto parentComp = m_ECS.GetMutableComponent<HierarchyComponent>(childComp->Parent);
		childComp->HierarchyLevel = 0;
		parentComp->Children.Remove(child);
	}

	const UnorderedCollection<EntityId, EntityHierarchy::SMALL_CHILDREN_SIZE>& EntityHierarchy::GetChildren(EntityId parent) const
	{
		auto hierarchyComp = m_ECS.GetComponent<HierarchyComponent>(parent);
		assert(hierarchyComp);
		return hierarchyComp->Children;
	}

	EntityId EntityHierarchy::GetParent(EntityId child)
	{
		auto hierarchy = m_ECS.GetComponent<HierarchyComponent>(child);
		assert(hierarchy);
		return hierarchy->Parent;
	}

	bool EntityHierarchy::TryGetParent(EntityId child, EntityId& outParent)
	{
		auto hierarchy = m_ECS.GetComponent<HierarchyComponent>(child);
		assert(hierarchy);
		if (hierarchy->HierarchyLevel > 0) {
			outParent = hierarchy->Parent;
			return true;
		}
		return false;
	}

	void EntityHierarchy::OnCreateEntity(EntityId createdEntity)
	{
		m_ECS.SetComponent(createdEntity, EntityIdComponent{ createdEntity });
		m_ECS.SetComponent(createdEntity, HierarchyComponent{});
	}
	void EntityHierarchy::RemoveEntityAndChildren(EntityId entity)
	{
		auto childComp = m_ECS.GetComponent<HierarchyComponent>(entity);
		assert(childComp);
		if (childComp->HierarchyLevel > 0) {
			// Let the parent know they don't have this child anymore
			auto parentComp = m_ECS.GetMutableComponent<HierarchyComponent>(childComp->Parent);
			assert(parentComp);
			auto& children = parentComp->Children;
			children.Remove(entity);
		}

		RemoveEntityRecursive(entity);
	}

	void EntityHierarchy::RemoveEntityRecursive(EntityId entity)
	{
		{
			// Remove any children
			auto trackChildrenComp = m_ECS.GetComponent<HierarchyComponent>(entity);
			assert(trackChildrenComp);
			auto& children = trackChildrenComp->Children;
			for (auto& child : children) {
				RemoveEntityRecursive(child);
			}

			m_ECS.DestroySingleEntity(entity);
		}
	}
}