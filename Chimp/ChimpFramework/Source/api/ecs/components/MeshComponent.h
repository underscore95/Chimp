#pragma once

#include "stdafx.h"
#include "api/graphics/meshes/Mesh.h"
#include "api/ecs/components/ComponentRegistry.h"

namespace Chimp {
	class MeshComponent {
	public:
		// Don't use this constructor, it's only here for the ECS to use
		[[deprecated]] MeshComponent() :
			Mesh(nullptr) {}

		MeshComponent(Mesh* mesh)
			: Mesh(mesh)
		{
		}

		MeshComponent(const MeshComponent& other)
			: Mesh(other.Mesh)
		{
		}

		Mesh* Mesh;
	};


	namespace Unused {
		static ComponentRegister<MeshComponent> MeshComponentRegister;
	}
}