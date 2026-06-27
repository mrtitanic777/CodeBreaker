#ifndef PACKET4UPDATETIME_H
#define PACKET4UPDATETIME_H

// Auto-generated from Packet4UpdateTime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet4UpdateTime : public Packet {
public:
	virtual ~Packet4UpdateTime();

	long long time;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET4UPDATETIME_H