#pragma once

#include "api/networking/IServer.h"
#include "api/networking/IClient.h"

namespace Chimp {
	class ENetServerFactory {
	public:
		static std::unique_ptr<IServer> CreateServer(const ConnectionInfo& serverInfo);
		static std::unique_ptr<IClient> CreateClient(const ConnectionInfo& serverInfo);
	};
}