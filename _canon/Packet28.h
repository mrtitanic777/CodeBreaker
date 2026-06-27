#ifndef PACKET28_H
#define PACKET28_H

// Auto-generated from Packet28.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class Entity;
class NetHandler;

class Packet28 : public Packet {
public:
	Packet28();
	Packet28(Entity* var1);
	Packet28(int var1, double var2, double var4, double var6);
	virtual ~Packet28();

	int entityId;
	int velocityX;
	int velocityY;
	int velocityZ;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET28_H