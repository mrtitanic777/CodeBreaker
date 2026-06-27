#ifndef PACKET60_H
#define PACKET60_H

// Auto-generated from Packet60.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <set>
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet60 : public Packet {
public:
	virtual ~Packet60();

	double explosionX;
	double explosionY;
	double explosionZ;
	float explosionSize;
	std::set<void*> destroyedBlockPositions;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();
};

#endif // PACKET60_H