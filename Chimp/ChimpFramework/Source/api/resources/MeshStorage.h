#pragma once

#include "stdafx.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "api/graphics/textures/ITexture.h"
#include "api/graphics/meshes/Mesh.h"
#include "api/utils/OptionalReference.h"

namespace Chimp {
	class ResourceManager;
	class Engine;
	// This is a utility class to store procedurally generated or hard coded meshes and models.
	// It's used to store meshes that weren't loaded from a model file, like TexturedQuad or other meshes such as a sphere.
	// Don't store actual models here, they are managed by the resource manager.
	// You can store and retrieve meshes, and reclaim or destroy them once you're done with them.
	class MeshStorage {
		friend class ResourceManager;
	private:
		MeshStorage(Engine& engine, ResourceManager& resourceManager);
	public:
		// Store a mesh, asset manager will keep it until its unloaded
		void StoreMesh(const std::string& id, std::unique_ptr<Mesh> mesh);
		// Create and store a textured quad mesh
		// id - id to store the mesh under
		// texturePath - path to the texture to use, will be loaded if it isn't already in the asset manager
		void CreateTexturedQuad(const std::string& id, const TextureResourcePath& texturePath);
		// Get mesh, bad stuff happens if you haven't stored it
		[[nodiscard]] Mesh& GetMesh(const std::string& id);
		// True if the mesh is stored
		[[nodiscard]] bool HasMesh(const std::string& id) const;
		// Reclaim a stored mesh, this will remove it from the asset manager
		std::unique_ptr<Mesh> ReclaimStoredMesh(const std::string& id);
		std::unique_ptr<Mesh> DestroyStoredMesh(const std::string& id) { return ReclaimStoredMesh(id); }

	private:
		Engine& m_Engine;
		std::unordered_map<std::string, std::unique_ptr<Mesh>> m_Meshes;
		ResourceManager& m_ResourceManager;
	};
}