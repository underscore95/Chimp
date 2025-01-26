#pragma once

#include "api/graphics/IRenderingManager.h"
#include "api/resources/ResourceDependency.h"

namespace Chimp {
	class TexturedQuad {
	public:
		TexturedQuad() = delete;

		// Create a textured quad mesh parallel to the XY plane centered at the origin.
		// Vertex layout:
		// 0: Position (3 floats)
		// 1: Texture coordinates (2 floats)
		// 
		// renderingManager - The rendering manager
		// texture - The texture to use
		static std::unique_ptr<Mesh> Create(
			IRenderingManager& renderingManager,
			const TextureDependency& texture
		);
	};
}