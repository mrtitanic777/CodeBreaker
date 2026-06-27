#ifndef PACKET29DESTROYENTITY_H
#define PACKET29DESTROYENTITY_H

// Auto-generated from Packet29DestroyEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet29DestroyEntity : public Packet {
public:
	virtual ~Packet29DestroyEntity();

	int entityId;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET29DESTROYENTITY_H