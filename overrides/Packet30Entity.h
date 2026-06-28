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
	// CONSOLE FIX (entities rocketing into the sky): these are Java `byte` = SIGNED. The converter's
	// typemap maps byte -> unsigned char, so a relative-move delta of -1 ("nudge down") was read as
	// +255 and serverPos += 255 launched every moving entity (mobs + remote players) ~8 blocks/packet
	// upward until it flew out of view (and the extreme coords likely triggered the IP-0 render crash).
	// Packet31RelEntityMove / Packet33RelEntityMoveLook inherit these, so fixing the base fixes all
	// relative-move packets. (A blanket typemap change is unsafe -- byte[] buffers want unsigned char.)
	signed char xPosition;
	signed char yPosition;
	signed char zPosition;
	signed char yaw;
	signed char pitch;
	bool rotating;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET30ENTITY_H