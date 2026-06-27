#ifndef PACKET12PLAYERLOOK_H
#define PACKET12PLAYERLOOK_H

// Auto-generated from Packet12PlayerLook.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet10Flying.h"

class DataInputStream;
class DataOutputStream;

class Packet12PlayerLook : public Packet10Flying {
public:
	Packet12PlayerLook();
	Packet12PlayerLook(float var1, float var2, bool var3);
	virtual ~Packet12PlayerLook();

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET12PLAYERLOOK_H