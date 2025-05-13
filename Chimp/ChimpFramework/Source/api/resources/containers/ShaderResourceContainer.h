#pragma once

#include "api/resources/ResourceContainer.h"
#include "api/graphics/shaders/IShader.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "Loggers.h"

namespace Chimp {
	class Engine;
	class ShaderResourceContainer : public ResourceContainer<ShaderFilePaths, IShader> {
	public:
		ShaderResourceContainer(Engine& engine) :
			ResourceContainer<ShaderFilePaths, IShader>([&](const ShaderFilePaths& paths) { return LoadResource(engine, paths); }) {

		}

		~ShaderResourceContainer() = default;

	private:
		Resource<IShader> LoadResource(Engine& engine, const ShaderFilePaths& paths);
	};
}