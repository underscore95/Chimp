#pragma once

#include "stdafx.h"

namespace Chimp {
	class IRenderingManager;
	class Engine;
	class LitShader;

	class ChimpShaders {
		friend class IRenderingManager;
	private:
		ChimpShaders(Engine& engine);

	public:
		~ChimpShaders();

	public:
		LitShader& GetLitShader();

	private:
		std::unique_ptr<LitShader> m_LitShader;
	};
}