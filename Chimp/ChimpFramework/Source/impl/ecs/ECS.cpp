#include "api/ecs/ECS.h"
#include "api/ecs/components/ComponentRegistry.h"
#include "api/ecs/components/EntityIdComponent.h"
#include "api/utils/StringUtils.h"
#include "api/utils/preprocessor/Casting.h"
#include "api/Engine.h"

namespace Chimp {

	std::string ECS::Serialise()
	{
		auto view = GetEntitiesWithComponents<EntityIdComponent>();
		Json json;

		// Store each entity json in json[entityId]
		for (auto& [entId] : view) {
			auto newId = flecs::entity(m_World, entId.Id);
			Json entityJson;
			// Store each component in json[entityId][componentTypeName]
			GetComponentsOnEntity(newId, [&entityJson, this](AnyReference comp) {
				ComponentRegistry::Instance().Serialise(*this, entityJson, comp);
				});

			size_t id = newId.id();
			json[std::to_string(id)] = entityJson; // fun fact originally i just did json[id] but id is just a random uint64_t so it allocated a 71gb array
		}

		return json.dump();
	}

	std::unique_ptr<ECS> ECS::Deserialise(Engine& engine, std::string_view json, bool disableScriptProcessing, bool removeEditorComponents)
	{
		std::unordered_set<std::string> editorComponents = {
			"Chimp::EulerRotationComponent"
		};

		std::unique_ptr<ECS> ecs = engine.CreateECS();
		if (disableScriptProcessing) ecs->GetScripts().DisableProcessing();

		Json parsed = Json::parse(json); // Parse json

		std::vector<DeserialisedEntity> entities;

		// Create all the entities
		for (const auto& pair : parsed.items()) {
			const auto& key = pair.key();

			// Get the entity id
			EntityId entId;
			if (!StringToSizeT(key, &entId)) {
				Loggers::ECS().Error(std::format("Failed to convert entity id {} into a size_t", key));
				continue;
			}
			ecs->CreateEntityWithoutComponents(entId);

			// Push entity info
			entities.push_back(DeserialisedEntity{
				.Id=entId,
				.Json = parsed[key]
				});
		}

		// Loop all components
		for (const auto& ent : entities) {
			// Add all components
			for (const auto& pair : ent.Json.items()) {
				std::string typeName = pair.key();
				if (removeEditorComponents && editorComponents.contains(typeName)) {
					continue;
				}
				ComponentRegistry::Instance().Deserialise(*ecs, ent.Id, typeName, ent.Json);
			}
		}

		// Notify systems
		ecs->GetScripts().OnDeserialise();

		return std::move(ecs);
	}

	void ECS::GetComponentsOnEntity(EntityId entity, const std::function<void(AnyReference)>& function)
	{
		ToEntity(entity).each([this, entity, &function](flecs::id id) {
			auto it = m_ComponentIdToTypeInfo.find(id.raw_id());
			assert(it != m_ComponentIdToTypeInfo.end());
			TypeInfo typeInfo = it->second;
			void* componentPtr = ToEntity(entity).get_mut(id);
			assert(componentPtr);
			function(AnyReference{ typeInfo,componentPtr });
			});
	}

	void ECS::CreateEntityWithoutComponents(EntityId idToUse)
	{
		m_World.make_alive(idToUse);
	}
}