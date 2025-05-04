#include "api/resources/containers/ModelResourceContainer.h"

namespace Chimp {
	Resource<Mesh> ModelResourceContainer::LoadResource(Engine& engine, const ModelResourcePath& paths)
	{
		return m_ModelImporter->LoadModel(paths, ImportSettings);
	}
}