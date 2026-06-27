#ifndef PACKET52MULTIBLOCKCHANGE_H
#define PACKET52MULTIBLOCKCHANGE_H

// Auto-generated from Packet52MultiBlockChange.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet52MultiBlockChange : public Packet {
public:
	Packet52MultiBlockChange();
	virtual ~Packet52MultiBlockChange();

	int xPosition;
	int zPosition;
	JArray<short > coordinateArray;
	JArray<unsigned char > typeArray;
	JArray<unsigned char > metadataArray;
	int size;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET52MULTIBLOCKCHANGE_H