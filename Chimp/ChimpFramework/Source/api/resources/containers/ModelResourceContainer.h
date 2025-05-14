#pragma once

#include "api/resources/ResourceContainer.h"
#include "api/graphics/meshes/Mesh.h"
#include "api/resources/IModelImporter.h"
#include "Loggers.h"
#include "impl/utils/NotNullHack.h"

namespace Chimp {
	class Engine;
	class ResourceManager;
	class ModelResourceContainer : public ResourceContainer<ModelResourcePath, Mesh> {
		friend class ResourceManager;
	private:
		// Special constructor for resource manager since it is a special bad coded class that doesn't have an importer before it needs a container.
		ModelResourceContainer(Engine& engine) :
			ResourceContainer<ModelResourcePath, Mesh>([&](const ModelResourcePath& path) { return LoadResource(engine, path); }) {
		}

	public:
		ModelResourceContainer(Engine& engine, IModelImporter& modelImporter) :
			ResourceContainer<ModelResourcePath, Mesh>([&](const ModelResourcePath& path) { return LoadResource(engine, path); })
		{
			m_ModelImporter = modelImporter;
		}

		~ModelResourceContainer() = default;

		IModelImporter::Settings ImportSettings; // Will be used whenever we load a model

		const IModelImporter& GetModelImporter() const { return m_ModelImporter; }

	private:
		Resource<Mesh> LoadResource(Engine& engine, const ModelResourcePath& paths);
		void OnUnload(const ModelResourcePath& path);

	private:
		Chimp::Reference<IModelImporter> m_ModelImporter;
	};
}