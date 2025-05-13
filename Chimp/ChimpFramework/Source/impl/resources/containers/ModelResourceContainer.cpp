#include "api/resources/containers/ModelResourceContainer.h"
#include "api/resources/ResourceReference.h"

namespace Chimp {
	Resource<Mesh> ModelResourceContainer::LoadResource(Engine& engine, const ModelResourcePath& path)
	{
		auto model = m_ModelImporter->LoadModel(path, ImportSettings);
		ImportedAssetsList::Instance().NotifyAssetLoaded(path, AnyReference{ *model });
		return std::move(model);
	}

	void ModelResourceContainer::OnUnload(const ImageResourcePath& path)
	{
		ImportedAssetsList::Instance().NotifyAssetUnloaded(path);
	}
}