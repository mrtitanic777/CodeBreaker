#ifndef PACKET20NAMEDENTITYSPAWN_H
#define PACKET20NAMEDENTITYSPAWN_H

// Auto-generated from Packet20NamedEntitySpawn.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class EntityPlayer;
class NetHandler;

class Packet20NamedEntitySpawn : public Packet {
public:
	Packet20NamedEntitySpawn();
	Packet20NamedEntitySpawn(EntityPlayer* var1);
	virtual ~Packet20NamedEntitySpawn();

	int entityId;
	JavaString name;
	int xPosition;
	int yPosition;
	int zPosition;
	unsigned char rotation;
	unsigned char pitch;
	int currentItem;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET20NAMEDENTITYSPAWN_H