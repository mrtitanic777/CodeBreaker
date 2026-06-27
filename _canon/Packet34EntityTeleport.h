#ifndef PACKET34ENTITYTELEPORT_H
#define PACKET34ENTITYTELEPORT_H

// Auto-generated from Packet34EntityTeleport.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class Entity;
class NetHandler;

class Packet34EntityTeleport : public Packet {
public:
	Packet34EntityTeleport();
	Packet34EntityTeleport(Entity* var1);
	virtual ~Packet34EntityTeleport();

	int entityId;
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

#endif // PACKET34ENTITYTELEPORT_H