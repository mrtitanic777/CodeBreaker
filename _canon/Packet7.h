#ifndef PACKET7_H
#define PACKET7_H

// Auto-generated from Packet7.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet7 : public Packet {
public:
	Packet7();
	Packet7(int var1, int var2, int var3);
	virtual ~Packet7();

	int playerEntityId;
	int targetEntityId;
	int isLeftClick;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET7_H