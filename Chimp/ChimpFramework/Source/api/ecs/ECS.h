#pragma once

#include "stdafx.h"
#include "api/utils/OptionalReference.h"
#include "EntityId.h"
#include "Loggers.h"
#include "transform/TransformManager.h"
#include "SystemContainerSystem.h"
#include "scripting/EntityScriptingSystem.h"
#include "api/utils/TypeInfo.h"
#include "api/utils/AnyReference.h"
#include "api/graphics/shaders/shaders/lighting/LightManager.h"
#include "hierarchy/EntityHierarchy.h"

namespace Chimp {
	struct DeserialisedEntity {
		EntityId Id;
		Json Json;
	};

	class Engine;
	class ComponentRegistry;
	struct EntityIdComponent;

#ifdef CHIMP_FLECS
	class ECS {
		friend class Engine;
		friend class ComponentRegistry;
		friend class EntityHierarchy;
	private:
		ECS(Engine& engine)
			: m_TransformManager(new TransformManager(*this)),
			m_SystemContainer(engine, *this),
			m_EntityHierarchy(*this),
			m_LightManager(*this)
		{
			{
				auto scriptingSystem = UNIQUE_PTR_CAST_FROM_RAW_PTR(ISystem, new EntityScriptingSystem(engine, *this));
				m_EntityScripting = static_cast<EntityScriptingSystem*>(scriptingSystem.get());
				m_SystemContainer.RegisterSystem(std::move(scriptingSystem));
			}
		}

	public:
		// Serialisation
		// Serialise to json
		std::string Serialise();

		// Deserialise from json
		static std::unique_ptr<ECS> Deserialise(Engine& engine, std::string_view json, bool disableScriptProcessing);

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
		EntityScriptingSystem& GetScripts() { return m_EntityScripting.Get(); }
		EntityHierarchy& GetHierarchy() { return m_EntityHierarchy; }
		LightManager& GetLightManager() { return m_LightManager; }

		// Get number of alive entities
		[[nodiscard]] size_t GetEntityCount() const {
			return m_EntityCount;
		}

		// Create an entity with minimal components
		Entity CreateEntity() {
			m_EntityCount++;
			auto ent = m_World.entity();
			m_EntityHierarchy.OnCreateEntity(ent);
			return ent;
		}

		// Remove an entity from the world
		void RemoveEntity(EntityId entity) {
			m_EntityHierarchy.RemoveEntityAndChildren(entity);
		}

		// Convert id to entity
		Entity ToEntity(EntityId entity) const {
			return IdToEntity(entity, m_World);
		}

		// Is entity alive
		bool IsEntityAlive(EntityId entity) const {
			return ToEntity(entity).is_alive();
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
				Loggers::Main().Error("Component " + name + " not registered. Perhaps you didn't include the header with the ComponentRegister?");
				assert(false);
			}

			assert(IsEntityAlive(entity));
#endif
			ToEntity(entity).set(component);
		}

		// Get a component from an entity
		// entity - The entity to get the component from
		template <typename Component>
		ConstOptionalReference<Component> GetComponent(EntityId entityId) const {
			auto entity = ToEntity(entityId);
			return ConstOptionalReference<Component>(entity.get<Component>());
		}

		// Get a component from an entity
		template <typename Component>
		OptionalReference<Component> GetMutableComponent(EntityId entity) {
			return OptionalReference<Component>(ToEntity(entity).get_mut<Component>());
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
		void GetComponentsOnEntity(EntityId entity, const std::function<void(AnyReference)>& function);

		TransformManager& GetTransformManager() {
			return *m_TransformManager;
		}

	private:

		void DestroySingleEntity(EntityId entity) {
			m_EntityCount--;
			ToEntity(entity).destruct();
		}


		// Used for ECS deserialisation only
		void CreateEntityWithoutComponents(EntityId idToUse);

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
		Reference<EntityScriptingSystem> m_EntityScripting;
		EntityHierarchy	m_EntityHierarchy;
		std::unordered_map<flecs::id_t, TypeInfo> m_ComponentIdToTypeInfo;
		LightManager m_LightManager;
#ifndef NDEBUG
		std::unordered_set<TypeInfo> m_RegisteredComponents;
#endif
	};
#endif
}