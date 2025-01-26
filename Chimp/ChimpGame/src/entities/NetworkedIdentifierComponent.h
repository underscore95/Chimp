#pragma once

typedef int NetworkId;

struct NetworkedIdentifierComponent {
public:
	// Guarantees a unique id to this client, it is possible two clients have the same id though
	// which is okay since if they are on different clients they are part of different simulations
	// only worry is if the server needed to store this, in which case it should store something like a tuple of client id and network id
	NetworkedIdentifierComponent() {
		Id = NextId++;
	}

	NetworkId Id;

private:
	static NetworkId NextId;
};