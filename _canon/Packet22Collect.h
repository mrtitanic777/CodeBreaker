#ifndef PACKET22COLLECT_H
#define PACKET22COLLECT_H

// Auto-generated from Packet22Collect.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet22Collect : public Packet {
public:
	virtual ~Packet22Collect();

	int collectedEntityId;
	int collectorEntityId;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET22COLLECT_H