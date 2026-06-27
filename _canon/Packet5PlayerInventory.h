#ifndef PACKET5PLAYERINVENTORY_H
#define PACKET5PLAYERINVENTORY_H

// Auto-generated from Packet5PlayerInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class ItemStack;
class NetHandler;

class Packet5PlayerInventory : public Packet {
public:
	Packet5PlayerInventory();
	Packet5PlayerInventory(int var1, JArray<ItemStack* > var2);
	virtual ~Packet5PlayerInventory();

	int type;
	JArray<ItemStack* > stacks;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET5PLAYERINVENTORY_H