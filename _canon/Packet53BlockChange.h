#ifndef PACKET53BLOCKCHANGE_H
#define PACKET53BLOCKCHANGE_H

// Auto-generated from Packet53BlockChange.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet53BlockChange : public Packet {
public:
	Packet53BlockChange();
	virtual ~Packet53BlockChange();

	int xPosition;
	int yPosition;
	int zPosition;
	int type;
	int metadata;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET53BLOCKCHANGE_H