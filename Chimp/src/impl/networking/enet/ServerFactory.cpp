#include "ServerFactory.h"
#include "Server.h"
#include "Client.h"

namespace Chimp {
	std::unique_ptr<IServer> ENetServerFactory::CreateServer(const ConnectionInfo& serverInfo) {
		return std::make_unique<Server>(serverInfo);
	}

	std::unique_ptr<IClient> ENetServerFactory::CreateClient(const ConnectionInfo& serverInfo)
	{
		return std::make_unique<Client>(serverInfo);
	}
}