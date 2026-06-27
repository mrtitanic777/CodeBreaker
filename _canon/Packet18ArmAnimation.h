#ifndef PACKET18ARMANIMATION_H
#define PACKET18ARMANIMATION_H

// Auto-generated from Packet18ArmAnimation.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class Entity;
class NetHandler;

class Packet18ArmAnimation : public Packet {
public:
	Packet18ArmAnimation();
	Packet18ArmAnimation(Entity* var1, int var2);
	virtual ~Packet18ArmAnimation();

	int entityId;
	int animate;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET18ARMANIMATION_H