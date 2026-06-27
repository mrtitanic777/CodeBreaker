#ifndef PACKET17ADDTOINVENTORY_H
#define PACKET17ADDTOINVENTORY_H

// Auto-generated from Packet17AddToInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet17AddToInventory : public Packet {
public:
	virtual ~Packet17AddToInventory();

	int id;
	int count;
	int durability;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET17ADDTOINVENTORY_H