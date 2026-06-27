#ifndef PACKET11PLAYERPOSITION_H
#define PACKET11PLAYERPOSITION_H

// Auto-generated from Packet11PlayerPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet10Flying.h"

class DataInputStream;
class DataOutputStream;

class Packet11PlayerPosition : public Packet10Flying {
public:
	Packet11PlayerPosition();
	Packet11PlayerPosition(double var1, double var3, double var5, double var7, bool var9);
	virtual ~Packet11PlayerPosition();

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET11PLAYERPOSITION_H