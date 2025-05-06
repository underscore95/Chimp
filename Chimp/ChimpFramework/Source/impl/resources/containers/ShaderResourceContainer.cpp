#include "api/resources/containers/ShaderResourceContainer.h"
#include "api/Engine.h"

namespace Chimp {
	Resource<IShader> ShaderResourceContainer::LoadResource(Engine& engine, const ShaderFilePaths& paths)
	{
		Resource<IShader> resource(engine.GetRenderingManager().CompileShader(paths));
		if (!resource.Data->IsValid())
		{
			std::stringstream ss;
			ss << "Failed to compile shader" << std::endl
				<< "Vertex: " << paths.Vertex << std::endl
				<< "Fragment: " << paths.Fragment << std::endl;
			Loggers::Resources().Error(ss.str());
		}
		return resource;
	}
}