#pragma once

#include "stdafx.h"
#include "api/utils/OptionalReference.h"
#include "EntityId.h"
#include "components/HierarchyComponent.h"
#include "Loggers.h"
#include "transform/TransformManager.h"
#include "SystemContainerSystem.h"
#include "scripting/EntityScriptingSystem.h"

namespace Chimp {
	class Engine;

#ifdef CHIMP_FLECS
	class ECS {
		friend class Engine;
	private:
		ECS(Engine& engine)
			: m_TransformManager(new TransformManager(*this)),
			m_SystemContainer(engine, *this),
			m_EntityScripting(*new EntityScriptingSystem(engine, *this))
		{
			m_SystemContainer.RegisterSystem(UNIQUE_PTR_CAST_FROM_RAW_PTR(ISystem, &m_EntityScripting));
		}

	public:
		// A view represents a set of entities each with the same common set of components
		template <typename... Components>
		class View {
			friend class ECS;
		private:
			using ComponentTuple = std::tuple<Components&...>;
			View(flecs::world& world) {
				auto system = world.system<Components...>().each([this](Components&... components) {
					m_Components.emplace_back(components...);
					});

				system.run();
				system.destruct();
			}

			View(std::vector<ComponentTuple>&& components) : m_Components(std::move(components)) {}

		public:
			~View() = default;

			// Remove all entities from the view that do not satisfy the predicate
			void WithPredicate(const std::function<bool(ComponentTuple&)>& predicate) {
				for (auto it = m_Components.begin(); it != m_Components.end();) {
					if (!predicate(*it)) {
						it = m_Components.erase(it);
					}
					else {
						++it;
					}
				}
			}

		public:
			using iterator = typename std::vector<ComponentTuple>::iterator;

			iterator begin() { return m_Components.begin(); }
			iterator end() { return m_Components.end(); }

			size_t Size() const {
				return m_Components.size();
			}

			ComponentTuple& GetByIndex(size_t index) {
				return m_Components[index];
			}

		private:
			std::vector<ComponentTuple> m_Components;
		};

	public:
		SystemContainerSystem& GetSystems() { return m_SystemContainer; }
		EntityScriptingSystem& GetScripts() { return m_EntityScripting; }

		void SetParent(EntityId child, EntityId parent) {
			if (child == parent) {
				Loggers::Main().Error(std::format("Attempted to make {} a parent of itself.", child.id()));
				assert(false);
				return;
			}

			// Check if we have a parent
			auto hierarchyComp = child.get_mut<HierarchyComponent>();
			assert(hierarchyComp);
			if (hierarchyComp->HasParent) {
				// TODO: Do nothing if the parent is the same?
				// We have a parent, let them know their child is leaving if they track children
				auto oldParentsComp = hierarchyComp->Parent.get_mut<HierarchyComponent>();
				assert(oldParentsComp);
				auto& children = oldParentsComp->Children;
				children.Remove(child);
			}

			// Set the new parent
			hierarchyComp->Parent = parent;
			hierarchyComp->HasParent = true;

			auto trackChildrenComp = parent.get_mut<HierarchyComponent>();
			assert(trackChildrenComp);
			auto& children = trackChildrenComp->Children;
			children.Insert(child);
		}

		bool IsChildOf(EntityId parent, EntityId possibleChild) const {
			auto trackChildrenComp = parent.get<HierarchyComponent>();
			assert(trackChildrenComp);
			auto& children = trackChildrenComp->Children;
			return children.Contains(possibleChild);
		}

		void RemoveChild(EntityId parent, EntityId child) const {
			assert(IsChildOf(parent, child));
			auto parentComp = parent.get_mut<HierarchyComponent>();
			auto childComp = child.get_mut<HierarchyComponent>();
			childComp->HasParent = false;
			parentComp->Children.Remove(child);
		}

		const UnorderedPossiblyUniqueCollection<EntityId, HierarchyComponent::SMALL_CHILDREN_SIZE>& GetChildren(EntityId parent) const {
			auto hierarchyComp = parent.get<HierarchyComponent>();
			assert(hierarchyComp);
			return hierarchyComp->Children;
		}

		EntityId GetParent(EntityId child) {
			auto hierarchy = child.get<HierarchyComponent>();
			assert(hierarchy);
			return hierarchy->Parent;
		}

		bool TryGetParent(EntityId child, EntityId& outParent) {
			auto hierarchy = child.get<HierarchyComponent>();
			assert(hierarchy);
			if (hierarchy->HasParent) {
				outParent = hierarchy->Parent;
				return true;
			}
			return false;
		}

		// Get number of alive entities
		[[nodiscard]] size_t GetEntityCount() {
			return m_EntityCount;
		}

		// Create an entity with minimal components
		EntityId CreateEntity() {
			m_EntityCount++;
			auto ent = m_World.entity();
			ent.set(HierarchyComponent{});
			return ent;
		}

		// Remove an entity from the world
		void RemoveEntity(EntityId entity) {
			auto childComp = entity.get<HierarchyComponent>();
			assert(childComp);
			if (childComp->HasParent) {
				// Let the parent know they don't have this child anymore
				auto parentComp = childComp->Parent.get_mut< HierarchyComponent>();
				assert(parentComp);
				auto& children = parentComp->Children;
				children.Remove(entity);
			}

			RemoveEntityRecursive(entity);
		}

		// Is entity alive
		bool IsEntityAlive(EntityId entity) {
			return entity.is_alive();
		}

		// Set a component on an entity, creating it if it doesn't exist
		// Component - The component type to set
		// entity - The entity to set the component on
		// component - The value to set the component to
		template <typename Component>
		void SetComponent(EntityId entity, const Component& component) {
			entity.set(component);
		}

		// Get a component from an entity
		// entity - The entity to get the component from
		template <typename Component>
		ConstOptionalReference<Component> GetComponent(EntityId entity) const {
			return ConstOptionalReference<Component>(entity.get<Component>());
		}

		// Get a component from an entity
		template <typename Component>
		OptionalReference<Component> GetMutableComponent(EntityId entity) {
			return OptionalReference<Component>(entity.get_mut<Component>());
		}

		// Get all entities with all of a set of components
		// Components - Types of components the entity should have
		// func - This function will be called once for every entity with all the specified components
		template <typename... Components>
		View<Components...> GetEntitiesWithComponents() {
			// This is slower than if we just passed a lambda in instead of returning a view
			// but it's only about 20-25% difference in release mode and imo the api is much nicer.
			return View<Components...>(m_World);
		}

		TransformManager& GetTransformManager() {
			return *m_TransformManager;
		}

	private:
		void RemoveEntityRecursive(EntityId entity) {
			// Remove any children
			auto trackChildrenComp = entity.get<HierarchyComponent>();
			assert(trackChildrenComp);
			auto& children = trackChildrenComp->Children;
			for (auto& child : children) {
				RemoveEntityRecursive(child);
			}

			m_EntityCount--;
			entity.destruct();
		}

	private:
		flecs::world m_World;
		size_t m_EntityCount = 0;
		std::unique_ptr<TransformManager> m_TransformManager;
		SystemContainerSystem m_SystemContainer;
		EntityScriptingSystem& m_EntityScripting;
	};
#endif
}