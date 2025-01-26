#pragma once

#include "stdafx.h"
#include "api/utils/OptionalReference.h"

#ifdef CHIMP_FLECS
#include <flecs.h>
#endif

namespace Chimp {
#ifdef CHIMP_FLECS
	typedef flecs::entity EntityId;
	class ECS {
	public:
		ECS() = default;

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
			void WithPredicate(const std::function<bool(ComponentTuple&)> &predicate) {
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

		// Get number of alive entities
		[[nodiscard]] size_t GetEntityCount() {
			return m_EntityCount;
		}

		// Create an entity with no components
		EntityId CreateEntity() {
			m_EntityCount++;
			return m_World.entity();
		}

		// Remove an entity from the world
		void RemoveEntity(EntityId entity) {
			m_EntityCount--;
			entity.destruct();
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
		ConstOptionalReference<Component> GetComponent(EntityId entity) {
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

	private:
		flecs::world m_World;
		size_t m_EntityCount = 0;
	};
#endif
}