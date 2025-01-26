#pragma once

#include "stdafx.h"
#include "ShaderTypes.h"
#include "api/utils/preprocessor/CopyAndMove.h"

namespace Chimp
{
	template <typename GraphicsID>
	class IShaderCompiler {
		DISABLE_COPY(IShaderCompiler);
	protected:
		IShaderCompiler() = default;

	public:
		~IShaderCompiler() = default;

		struct Result {
			bool Success;
			GraphicsID Id;
		};
		virtual Result Compile(const ShaderType type, const std::string &source) const = 0;
	};
}