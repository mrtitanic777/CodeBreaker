#ifndef PACKET23VEHICLESPAWN_H
#define PACKET23VEHICLESPAWN_H

// Auto-generated from Packet23VehicleSpawn.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet23VehicleSpawn : public Packet {
public:
	virtual ~Packet23VehicleSpawn();

	int entityId;
	int xPosition;
	int yPosition;
	int zPosition;
	int type;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET23VEHICLESPAWN_H