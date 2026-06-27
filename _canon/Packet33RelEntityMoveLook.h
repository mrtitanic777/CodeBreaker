#ifndef PACKET33RELENTITYMOVELOOK_H
#define PACKET33RELENTITYMOVELOOK_H

// Auto-generated from Packet33RelEntityMoveLook.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet30Entity.h"

class DataInputStream;
class DataOutputStream;

class Packet33RelEntityMoveLook : public Packet30Entity {
public:
	Packet33RelEntityMoveLook();
	virtual ~Packet33RelEntityMoveLook();

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET33RELENTITYMOVELOOK_H