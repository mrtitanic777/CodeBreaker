#ifndef PACKET31RELENTITYMOVE_H
#define PACKET31RELENTITYMOVE_H

// Auto-generated from Packet31RelEntityMove.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet30Entity.h"

class DataInputStream;
class DataOutputStream;

class Packet31RelEntityMove : public Packet30Entity {
public:
	virtual ~Packet31RelEntityMove();

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET31RELENTITYMOVE_H