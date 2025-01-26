#include "InitENet.h"

namespace Chimp {
	void InitENet::Init()
	{
		if (!m_IsInitialized)
		{
			m_IsInitialized = true;
			if (enet_initialize() != 0)
			{
				std::cerr << "An error occurred while initializing ENet." << std::endl;
				assert(false);
				exit(-1);
			}
			atexit(enet_deinitialize);
		}
	}

	bool InitENet::m_IsInitialized = false;
}