#ifndef PACKET2HANDSHAKE_H
#define PACKET2HANDSHAKE_H

// Auto-generated from Packet2Handshake.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet2Handshake : public Packet {
public:
	Packet2Handshake();
	Packet2Handshake(const JavaString& var1);
	virtual ~Packet2Handshake();

	JavaString username;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET2HANDSHAKE_H