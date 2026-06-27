#ifndef PACKET16BLOCKITEMSWITCH_H
#define PACKET16BLOCKITEMSWITCH_H

// Auto-generated from Packet16BlockItemSwitch.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet16BlockItemSwitch : public Packet {
public:
	Packet16BlockItemSwitch();
	Packet16BlockItemSwitch(int var1, int var2);
	virtual ~Packet16BlockItemSwitch();

	int unused;
	int id;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET16BLOCKITEMSWITCH_H