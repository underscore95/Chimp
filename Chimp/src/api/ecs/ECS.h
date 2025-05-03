#pragma once

#include "stdafx.h"
#include "api/utils/OptionalReference.h"
#include "EntityId.h"
#include "components/TracksChildrenComponent.h"
#include "components/ParentComponent.h"

namespace Chimp {
#ifdef CHIMP_FLECS
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

		void SetParent(EntityId child, EntityId parent) {
			{
				// Check if we have a parent
				auto parentComp = child.get<ParentComponent>();
				if (parentComp) {
					// TODO: Do nothing if the parent is the same?
					// We have a parent, let them know their child is leaving if they track children
					auto trackChildrenComp = parentComp->Parent.get_mut< TracksChildrenComponent>();
					if (trackChildrenComp) {
						auto& children = trackChildrenComp->Children;
						children.erase(child);
					}
				}
			}

			// Set the new parent
			child.set(ParentComponent{ parent });

			// Does the new parent track children?
			auto trackChildrenComp = parent.get_mut< TracksChildrenComponent>();
			if (trackChildrenComp) {
				auto& children = trackChildrenComp->Children;
				children.insert(child);
			}
		}

		void AddChild(EntityId parent, EntityId child) {
			assert(!IsChildOf(parent, child));
			auto trackChildrenComp = parent.get_mut< TracksChildrenComponent>();
			trackChildrenComp->Children.insert(child);
		}

		void RemoveChild(EntityId parent, EntityId child) {
			assert(IsChildOf(parent, child));
			auto trackChildrenComp = parent.get_mut< TracksChildrenComponent>();
			trackChildrenComp->Children.erase(child);
		}

		bool IsChildOf(EntityId parent, EntityId possibleChild) const {
			auto trackChildrenComp = parent.get< TracksChildrenComponent>();
			assert(trackChildrenComp);
			auto& children = trackChildrenComp->Children;
			return children.contains(possibleChild);
		}

		const std::set<EntityId>& GetChildren(EntityId parent) const {
			auto trackChildrenComp = parent.get< TracksChildrenComponent>();
			assert(trackChildrenComp);
			return trackChildrenComp->Children;
		}

		EntityId GetParent(EntityId child) {
			auto parent = child.get<ParentComponent>();
			assert(parent);
			return parent->Parent;
		}

		// Get number of alive entities
		[[nodiscard]] size_t GetEntityCount() {
			return m_EntityCount;
		}

		// Create an entity with no components
		EntityId CreateEntity() {
			m_EntityCount++;
			return m_World.entity();
		}

		// Create an entity with a TracksChildrenComponent
		EntityId CreateEntityAndTrackChildren() {
			EntityId ent = CreateEntity();
			ent.set(TracksChildrenComponent{});
			return ent;
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
			static_assert(!IsHierachyComponent_v<Component>);
			entity.set(component);
		}

		// Get a component from an entity
		// entity - The entity to get the component from
		template <typename Component>
		ConstOptionalReference<Component> GetComponent(EntityId entity) const {
			static_assert(!IsHierachyComponent_v<Component>);
			return ConstOptionalReference<Component>(entity.get<Component>());
		}

		// Get a component from an entity
		template <typename Component>
		OptionalReference<Component> GetMutableComponent(EntityId entity) {
			static_assert(!IsHierachyComponent_v<Component>);
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
			template <typename Component>
			inline static constexpr bool IsHierachyComponent_v =
				std::is_base_of<TracksChildrenComponent, Component>::value ||
				std::is_base_of<ParentComponent, Component>::value;

	private:
		flecs::world m_World;
		size_t m_EntityCount = 0;
	};
#endif
}