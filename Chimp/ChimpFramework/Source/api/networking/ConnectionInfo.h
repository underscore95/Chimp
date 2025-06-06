#pragma once

namespace Chimp {
	struct ConnectionInfo
	{
	public:
		unsigned short Port = 48484;
		unsigned int MaxClients = 32; // Ignored if IsHost is false
		unsigned int MaxChannels = 2;
		unsigned int MaxIncomingBandwidth = 0; // 0 = unlimited
		unsigned int MaxOutgoingBandwidth = 0; // 0 = unlimited
		std::string HostName = "localhost"; // If IsHost is true, this must be "localhost"
		unsigned int ConnectionTimeoutMs = 5000;
		unsigned int KeepAliveIntervalMs = 1000;
		unsigned int FailedPacketsBeforeDisconnect = 3;

		// Returns true if the server info is valid
		bool IsValid() const {
			if (MaxClients == 0)
				return false; // No clients
			if (MaxChannels == 0)
				return false; // No channels
			if (ConnectionTimeoutMs == 0)
				return false; // Connection timeout should be greater than 0
			return true;
		}
	};
}