#pragma once

#include "IShadowMap.h"

namespace Chimp {
	class ICubeShadowMap : public IShadowMap {
	public:
		ICubeShadowMap(unsigned int size) : IShadowMap(size, size) {}
	};
}