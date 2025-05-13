#pragma once

#include "stdafx.h"
#include "api/graphics/meshes/Mesh.h"
#include "api/ecs/components/ComponentRegistry.h"
#include "api/resources/ResourceReference.h"

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
				auto str = std::format("Mesh: {}", comp.Mesh ? comp.Mesh.Get()->GetName() : "Resource Not Imported");
				ImGui::TextWrapped(str.c_str());
			}
		};
		COMPONENT_REGISTER(MeshComponentRegister);
	}
}