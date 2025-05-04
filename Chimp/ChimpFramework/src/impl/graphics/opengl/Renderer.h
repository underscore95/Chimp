#pragma once

#include "api/graphics/IRenderer.h"

namespace Chimp::GL {
	class Renderer : public IRenderer
	{
	public:
		Renderer() = default;
		~Renderer() override = default;

		void Draw(const Mesh::Section& meshSection, const IShader& shader) const override;

		void SetClearColor(float r, float g, float b) const override;

	protected:
		void StartDrawing() const override;
	};
}