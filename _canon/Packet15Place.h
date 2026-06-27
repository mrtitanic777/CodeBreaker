#ifndef PACKET15PLACE_H
#define PACKET15PLACE_H

// Auto-generated from Packet15Place.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet15Place : public Packet {
public:
	Packet15Place();
	Packet15Place(int var1, int var2, int var3, int var4, int var5);
	virtual ~Packet15Place();

	int id;
	int xPosition;
	int yPosition;
	int zPosition;
	int direction;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET15PLACE_H