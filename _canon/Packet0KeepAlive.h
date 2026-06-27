#ifndef PACKET0KEEPALIVE_H
#define PACKET0KEEPALIVE_H

// Auto-generated from Packet0KeepAlive.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet0KeepAlive : public Packet {
public:
	virtual ~Packet0KeepAlive();

	virtual void processPacket(NetHandler* var1);
	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET0KEEPALIVE_H