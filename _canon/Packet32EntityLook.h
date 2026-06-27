#ifndef PACKET32ENTITYLOOK_H
#define PACKET32ENTITYLOOK_H

// Auto-generated from Packet32EntityLook.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet30Entity.h"

class DataInputStream;
class DataOutputStream;

class Packet32EntityLook : public Packet30Entity {
public:
	Packet32EntityLook();
	virtual ~Packet32EntityLook();

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET32ENTITYLOOK_H