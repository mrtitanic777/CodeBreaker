#ifndef PACKET21PICKUPSPAWN_H
#define PACKET21PICKUPSPAWN_H

// Auto-generated from Packet21PickupSpawn.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class EntityItem;
class NetHandler;

class Packet21PickupSpawn : public Packet {
public:
	Packet21PickupSpawn();
	Packet21PickupSpawn(EntityItem* var1);
	virtual ~Packet21PickupSpawn();

	int entityId;
	int xPosition;
	int yPosition;
	int zPosition;
	unsigned char rotation;
	unsigned char pitch;
	unsigned char roll;
	int itemId;
	int count;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET21PICKUPSPAWN_H