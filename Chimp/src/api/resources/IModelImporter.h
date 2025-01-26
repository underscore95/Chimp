#pragma once

#include "stdafx.h"
#include "api/graphics/meshes/Mesh.h"

namespace Chimp {
	class ResourceManager;
	class IRenderingManager;
	class IModelImporter {
	public:

		struct Settings {
			bool FlipUVs = false;
			bool IncludeNormals = false;
			bool IncludeTextureCoordinates = true;
		};
	protected:
		DISABLE_COPY(IModelImporter);
		IModelImporter(IRenderingManager& renderingManager) :
			m_RenderingManager(renderingManager) {}
	public:
		virtual ~IModelImporter() = default;

		// Load a model from a file, this model can contain multiple meshes with 1 texture per mesh.
		// path - The path to the model file
		// Returns a unique pointer to an Mesh struct containing (and owning) the mesh and a dependency on associated textures
		// Returns nullptr if the model couldn't be loaded
		// Uses float for position, normal and texture coordinates
		// Uses unsigned int for indices
		// Only support 1 texture per mesh section (one model (mesh in chimp) can have multiple mesh sections!)
		virtual std::unique_ptr<Mesh> LoadModel(const std::string& path, const Settings& settings = {}) = 0;

	protected:
		IRenderingManager& m_RenderingManager;
	};
}