#ifndef PACKET6SPAWNPOSITION_H
#define PACKET6SPAWNPOSITION_H

// Auto-generated from Packet6SpawnPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet6SpawnPosition : public Packet {
public:
	virtual ~Packet6SpawnPosition();

	int xPosition;
	int yPosition;
	int zPosition;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET6SPAWNPOSITION_H