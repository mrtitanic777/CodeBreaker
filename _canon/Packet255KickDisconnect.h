#ifndef PACKET255KICKDISCONNECT_H
#define PACKET255KICKDISCONNECT_H

// Auto-generated from Packet255KickDisconnect.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet255KickDisconnect : public Packet {
public:
	Packet255KickDisconnect();
	Packet255KickDisconnect(const JavaString& var1);
	virtual ~Packet255KickDisconnect();

	JavaString reason;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET255KICKDISCONNECT_H