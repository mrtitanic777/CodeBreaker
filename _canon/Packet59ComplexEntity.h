#ifndef PACKET59COMPLEXENTITY_H
#define PACKET59COMPLEXENTITY_H

// Auto-generated from Packet59ComplexEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NBTTagCompound;
class NetHandler;
class TileEntity;

class Packet59ComplexEntity : public Packet {
public:
	Packet59ComplexEntity();
	Packet59ComplexEntity(int var1, int var2, int var3, TileEntity* var4);
	virtual ~Packet59ComplexEntity();

	int xPosition;
	int yPosition;
	int zPosition;
	JArray<unsigned char > entityData;
	NBTTagCompound* entityNBT;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	Packet59ComplexEntity(const Packet59ComplexEntity&);
	Packet59ComplexEntity& operator=(const Packet59ComplexEntity&);
};

#endif // PACKET59COMPLEXENTITY_H