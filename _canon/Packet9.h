#ifndef PACKET9_H
#define PACKET9_H

// Auto-generated from Packet9.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet9 : public Packet {
public:
	virtual ~Packet9();

	virtual void processPacket(NetHandler* var1);
	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET9_H