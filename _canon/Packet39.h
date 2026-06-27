#ifndef PACKET39_H
#define PACKET39_H

// Auto-generated from Packet39.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet39 : public Packet {
public:
	virtual ~Packet39();

	int entityId;
	int vehicleEntityId;

	virtual int getPacketSize();
	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);

	static void initStatics();
};

#endif // PACKET39_H