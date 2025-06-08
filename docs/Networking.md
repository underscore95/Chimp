## Networking

Chimp includes built-in support for basic TCP networking, including server hosting, client connections, packet handling, and request/response communication.

---

### Connecting and Hosting

Use the engine to start a server or connect to one:

```cpp
engine.HostServer(port);                   // Starts a local server
engine.ConnectToServer(ip, port);          // Connects to a server
```

---

### Packet Format and Transmission

Packets are sent as binary blobs, but **every packet must start with a 4-byte `Chimp::NetworkPacketType`** which identifies its type.

Before using a custom packet type, you **must** register it:

```cpp
Chimp::PacketTypeRegistry::RegisterPacketType(YOUR_PACKET_ID);
```

- `YOUR_PACKET_ID` must be a unique `int`.
- **Do not use negative values**, those are reserved by Chimp.

---

### Sending and Receiving Packets

You can send packets using Chimp’s networking API. See the `ChimpGame/networking` folder for examples.

To send a packet and expect a response:

```cpp
engine.GetNetworkManager().SendRequest(
    myPacket,
    [](const SomeResponsePacket& response) {
        // Handle the response
    }
);
```

**Warning:** If the receiving end doesn’t send a response, this will leak memory (`sizeof(std::function)` per request).

---

### Packet Forwarding

The server supports packet forwarding, allowing messages from one client to be routed to another. This simulates peer-to-peer communication without actual P2P connections.

---

### Underlying Protocol

Chimp uses **TCP** as its transport layer:

- Guarantees ordered and reliable delivery.
- Packet boundaries are not preserved (ENet may split/merge packets), but this is abstracted away from you.

---

### Events

Chimp has an event system, however it is not coupled to the networking system so the documentation is separate, you should read the events documentation next.

---