#pragma once

#include "api/graphics/utils/IRenderTexture.h"

namespace Chimp {
	class ICubeShadowMap : public IRenderTexture {
	public:
		ICubeShadowMap(unsigned int size) : IRenderTexture(size, size) {}
	};
}