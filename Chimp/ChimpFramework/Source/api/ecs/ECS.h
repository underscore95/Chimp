#pragma once

#include "stdafx.h"
#include "api/utils/OptionalReference.h"
#include "EntityId.h"
#include "components/HierarchyComponent.h"
#include "Loggers.h"
#include "transform/TransformManager.h"
#include "SystemContainerSystem.h"
#include "scripting/EntityScriptingSystem.h"
#include "components/EntityIdComponent.h"
#include "api/utils/TypeInfo.h"
#include "api/utils/AnyReference.h"

namespace Chimp {
	class Engine;
	class ComponentRegistry;

#ifdef CHIMP_FLECS
	class ECS {
		friend class Engine;
		friend class ComponentRegistry;
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
			if (hierarchyComp->HierarchyLevel > 0) {
				// TODO: Do nothing if the parent is the same?
				// We have a parent, let them know their child is leaving if they track children
				auto oldParentsComp = hierarchyComp->Parent.get_mut<HierarchyComponent>();
				assert(oldParentsComp);
				auto& children = oldParentsComp->Children;
				children.Remove(child);
			}

			// Set the new parent
			hierarchyComp->Parent = parent;

			auto parentComp = parent.get_mut<HierarchyComponent>();
			assert(parentComp);
			auto& children = parentComp->Children;
			children.Insert(child);

			hierarchyComp->HierarchyLevel = parentComp->HierarchyLevel + 1;
		}

		bool IsChildOf(EntityId parent, EntityId possibleChild) const {
			auto trackChildrenComp = parent.get<HierarchyComponent>();
			assert(trackChildrenComp);
			auto& children = trackChildrenComp->Children;
			return children.Contains(possibleChild);
		}

		void OrphanChild(EntityId child) const {
			auto childComp = child.get_mut<HierarchyComponent>();
			assert(childComp);
			if (childComp->HierarchyLevel <= 0) return;
			auto parentComp = childComp->Parent.get_mut<HierarchyComponent>();
			childComp->HierarchyLevel = 0;
			parentComp->Children.Remove(child);
		}

		const UnorderedCollection<EntityId, HierarchyComponent::SMALL_CHILDREN_SIZE>& GetChildren(EntityId parent) const {
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
			if (hierarchy->HierarchyLevel > 0) {
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
			ent.set(EntityIdComponent{ ent });
			return ent;
		}

		// Remove an entity from the world
		void RemoveEntity(EntityId entity) {
			auto childComp = entity.get<HierarchyComponent>();
			assert(childComp);
			if (childComp->HierarchyLevel > 0) {
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
#ifndef NDEBUG
			if (!m_RegisteredComponents.contains(typeid(Component))) {
				std::string name = typeid(Component).name();
				Loggers::Main().Error("Component " + name + " not registered.");
				assert(false);
			}
#endif
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

		// Get all components on an entity
		void GetComponentsOnEntity(EntityId entity, const std::function<void(AnyConstReference)>& function);

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

		template <typename T>
		void RegisterComponent() {
			m_World.component<T>();
			m_ComponentIdToTypeInfo[m_World.id<T>()] = typeid(T);
#ifndef NDEBUG
			m_RegisteredComponents.insert(typeid(T));
#endif
		}

	private:
		flecs::world m_World;
		size_t m_EntityCount = 0;
		std::unique_ptr<TransformManager> m_TransformManager;
		SystemContainerSystem m_SystemContainer;
		EntityScriptingSystem& m_EntityScripting;
		std::unordered_map<flecs::id_t, TypeInfo> m_ComponentIdToTypeInfo;
#ifndef NDEBUG
		std::unordered_set< TypeInfo> m_RegisteredComponents;
#endif
	};
#endif
}