#ifndef PACKET30ENTITY_H
#define PACKET30ENTITY_H

// Auto-generated from Packet30Entity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet30Entity : public Packet {
public:
	Packet30Entity();
	virtual ~Packet30Entity();

	int entityId;
	unsigned char xPosition;
	unsigned char yPosition;
	unsigned char zPosition;
	unsigned char yaw;
	unsigned char pitch;
	bool rotating;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET30ENTITY_H