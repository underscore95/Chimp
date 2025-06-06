#include "api/graphics/shaders/shaders/ChimpShaders.h"
#include "api/graphics/shaders/shaders/LitShader.h"

namespace Chimp {
	ChimpShaders::ChimpShaders(Engine& engine) 	:
		m_LitShader(new LitShader(engine))
	{
	}

	ChimpShaders::~ChimpShaders()
	{
	}

	LitShader& ChimpShaders::GetLitShader()
	{
		return *m_LitShader;
	}
}