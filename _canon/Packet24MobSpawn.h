#ifndef PACKET24MOBSPAWN_H
#define PACKET24MOBSPAWN_H

// Auto-generated from Packet24MobSpawn.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class EntityLiving;
class NetHandler;

class Packet24MobSpawn : public Packet {
public:
	Packet24MobSpawn();
	Packet24MobSpawn(EntityLiving* var1);
	virtual ~Packet24MobSpawn();

	int entityId;
	unsigned char type;
	int xPosition;
	int yPosition;
	int zPosition;
	unsigned char yaw;
	unsigned char pitch;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET24MOBSPAWN_H