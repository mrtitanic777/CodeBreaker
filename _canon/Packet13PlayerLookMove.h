#ifndef PACKET13PLAYERLOOKMOVE_H
#define PACKET13PLAYERLOOKMOVE_H

// Auto-generated from Packet13PlayerLookMove.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet10Flying.h"

class DataInputStream;
class DataOutputStream;

class Packet13PlayerLookMove : public Packet10Flying {
public:
	Packet13PlayerLookMove();
	Packet13PlayerLookMove(double var1, double var3, double var5, double var7, float var9, float var10, bool var11);
	virtual ~Packet13PlayerLookMove();

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET13PLAYERLOOKMOVE_H