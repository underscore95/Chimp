#pragma once

#include "stdafx.h"
#include <enet/enet.h>

namespace Chimp {
	class InitENet {
	public:
		static void Init();
	private:
		static bool m_IsInitialized;
	};
}