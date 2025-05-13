#include "api/resources/MeshStorage.h"
#include "api/Engine.h"
#include "Loggers.h"
#include "api/graphics/meshes/TexturedQuad.h"
#include "api/resources/ResourceManager.h"
#include "api/resources/ResourceDependency.h"

namespace Chimp {
	MeshStorage::MeshStorage(Engine& engine, ResourceManager& resourceManager)
		: m_Engine(engine), m_ResourceManager(resourceManager)
	{
	}

	void MeshStorage::StoreMesh(const std::string& id, std::unique_ptr<Mesh> mesh)
	{
		if (m_Meshes.find(id) != m_Meshes.end())
		{
			Loggers::Resources().Error("Mesh already exists: " + id);
			return;
		}
		m_Meshes[id] = std::move(mesh);
	}

	void MeshStorage::CreateTexturedQuad(const std::string& id, const TextureResourcePath& texturePath)
	{
		auto mesh = TexturedQuad::Create(
			m_Engine.GetRenderingManager(),
			TextureDependency{ m_ResourceManager.GetTextures(), texturePath }
		);
		StoreMesh(id, std::move(mesh));
	}

	Mesh& MeshStorage::GetMesh(const std::string& id)
	{
		auto it = m_Meshes.find(id);
		assert(it != m_Meshes.end());
		return *it->second;
	}

	bool MeshStorage::HasMesh(const std::string& id) const
	{
		return m_Meshes.find(id) != m_Meshes.end();
	}

	std::unique_ptr<Mesh> MeshStorage::ReclaimStoredMesh(const std::string& id)
	{
		auto it = m_Meshes.find(id);
		assert(it != m_Meshes.end());
		std::unique_ptr<Mesh> mesh = std::move(it->second);
		m_Meshes.erase(it);
		return std::move(mesh);
	}
}