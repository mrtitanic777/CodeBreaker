#ifndef PACKET50PRECHUNK_H
#define PACKET50PRECHUNK_H

// Auto-generated from Packet50PreChunk.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet50PreChunk : public Packet {
public:
	virtual ~Packet50PreChunk();

	int xPosition;
	int yPosition;
	bool mode;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET50PRECHUNK_H