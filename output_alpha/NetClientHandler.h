#ifndef NETCLIENTHANDLER_H
#define NETCLIENTHANDLER_H
// PORT stub: net.minecraft.client.NetClientHandler.
//
// This is a SINGLEPLAYER Xbox 360 port. Multiplayer networking will be
// redesigned later against the 360 network API. The real net infrastructure
// (NetClientHandler, NetworkManager, NetHandler, Packet* hierarchy) is in
// skip_files and is NOT generated. This minimal stub only lets the MP consumer
// classes (EntityClientPlayerMP, GuiConnecting, GuiDownloadTerrain,
// PlayerControllerMP, WorldClient) COMPILE. Every method is a no-op.
//
// PORT: 360 MP API TODO -- replace with a real handler once MP is implemented.

#include <string>

class NetClientHandler {
public:
	NetClientHandler() : field_1209_a("") {}
	virtual ~NetClientHandler() {}

	// Queue an outbound packet. Takes void* so callers never need a real
	// Packet type; the no-op simply drops it (caller-allocated, leaked-on-paper
	// but these call sites are stubbed out, so nothing is actually passed).
	virtual void addToSendQueue(void* /*packet*/) {}

	// Pump inbound packets -- no-op until MP exists.
	virtual void processReadPackets() {}

	// Tear down the connection -- no-op.
	virtual void disconnect() {}

	// Status line shown by GuiConnecting while "logging in".
	std::string field_1209_a;
};

#endif // NETCLIENTHANDLER_H
