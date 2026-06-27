#ifndef PACKET10FLYING_H
#define PACKET10FLYING_H

// Auto-generated from Packet10Flying.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet10Flying : public Packet {
public:
	Packet10Flying();
	Packet10Flying(bool var1);
	virtual ~Packet10Flying();

	double xPosition;
	double yPosition;
	double zPosition;
	double stance;
	float yaw;
	float pitch;
	bool onGround;
	bool moving;
	bool rotating;

	virtual void processPacket(NetHandler* var1);
	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET10FLYING_H