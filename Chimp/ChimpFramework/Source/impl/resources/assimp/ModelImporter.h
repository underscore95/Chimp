#pragma once

#include "api/resources/IModelImporter.h"

namespace Chimp {
	class ResourceManager;
	class ModelImporter : public Chimp::IModelImporter {
		friend class ResourceManager;
	private:
		ModelImporter(
			IRenderingManager& renderingManager,
			ResourceManager& resourceManager) :
			IModelImporter(renderingManager),
			m_ResourceManager(resourceManager)
		{

		}
	public:
		virtual ~ModelImporter() = default;

		std::unique_ptr<Mesh> LoadModel(const std::string& path, const Settings& settings = {}) override;

	private:
		Assimp::Importer m_Importer;
		ResourceManager& m_ResourceManager;
	};
}