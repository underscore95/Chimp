#pragma once

#include "stdafx.h"
#include "api/graphics/meshes/Mesh.h"
#include "api/ecs/components/ComponentRegistry.h"
#include "api/resources/ResourceReference.h"
#include "api/resources/asset_types/AssetTypeManager.h"
#include "api/Engine.h"
#include "api/utils/StringUtils.h"
#include "Loggers.h"

namespace Chimp {
	class MeshComponent {
	public:
		// Don't use this constructor, it's only here for the ECS to use
		[[deprecated]] MeshComponent() :
			Mesh() {
		}

		MeshComponent(ResourceReference<Mesh> mesh)
			: Mesh(mesh)
		{
		}

		MeshComponent(const MeshComponent& other)
			: Mesh(other.Mesh)
		{
		}

		ResourceReference<Mesh> Mesh;
	};

	namespace Unused {
		class MeshComponentRegister : public ComponentRegister<MeshComponent> {
		public:
			MeshComponentRegister() : ComponentRegister() {
			}

			void RenderInspectorUI(EntityId id, MeshComponent& comp) override {
				auto assetType = Engine::GetEngine().GetResourceManager().GetAssetTypeManager().GetType(AssetTypeId::Model);
				std::vector<std::filesystem::path> assets = assetType->GetImportedAssets();

#ifdef CHIMP_RESOURCE_SERIALISATION
				std::string currentlySelectedResource = comp.Mesh ? comp.Mesh.GetPath().string() : "None";
				RemoveUntilAndIncluding("Data/", currentlySelectedResource);

				if (ImGui::BeginCombo("Mesh", currentlySelectedResource.c_str())) {
					if (ImGui::Selectable("None", !comp.Mesh)) {
						// Select none
						comp.Mesh.Set(nullptr, "");
					}

					// List of resources
					for (const auto& path : assets) {
						auto shortenedPathString = path.string();
						RemoveUntilAndIncluding("Data/", shortenedPathString);
						if (ImGui::Selectable(shortenedPathString.c_str(), comp.Mesh.IsEqual(shortenedPathString))) {
							int assetIndex = ImportedAssetsList::Instance().GetAssetIndex(path);
							comp.Mesh.Set(ImportedAssetsList::Instance().GetAsset<Mesh>(assetIndex), path);
						}
					}
					ImGui::EndCombo();
				}
#else
				ImGui::Text("Resource selection dropdown cannot render in release builds.");
#endif
			}

			MeshComponent Deserialise(const Json& json) override {
				std::string path = json["Path"];
				return { {	Engine::GetEngine().GetResourceManager().GetModels().ImmediateDepend(path), path} };
			}

			void Serialise(Json& json, const MeshComponent& comp) override {
#ifdef CHIMP_RESOURCE_SERIALISATION
				json["Path"] = comp.Mesh.GetPath();
#else
				Loggers::ECS().Warning("Cannot serialise MeshComponent unless CHIMP_RESOURCE_SERIALISATION is defined.");
#endif
			}
		};
		COMPONENT_REGISTER(MeshComponentRegister);
	}
}