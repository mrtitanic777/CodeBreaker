#ifndef PACKET14BLOCKDIG_H
#define PACKET14BLOCKDIG_H

// Auto-generated from Packet14BlockDig.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet14BlockDig : public Packet {
public:
	Packet14BlockDig();
	Packet14BlockDig(int var1, int var2, int var3, int var4, int var5);
	virtual ~Packet14BlockDig();

	int xPosition;
	int yPosition;
	int zPosition;
	int face;
	int status;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET14BLOCKDIG_H