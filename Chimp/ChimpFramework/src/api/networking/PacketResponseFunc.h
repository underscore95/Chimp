#pragma once

#include "stdafx.h"
#include "events/NetworkEventType.h"

namespace Chimp {
	// Represents a function that takes a packet and returns a packet that is a response to the original packet
	// this function should only be called with one type of packet
	// If a client is the one responding, this will be called with nullptr in the event the client disconnects before responding
	typedef std::function<std::unique_ptr<NetworkPacket>(const NetworkPacket*)> PacketResponseFunc;
}