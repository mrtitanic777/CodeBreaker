#ifndef PACKET1LOGIN_H
#define PACKET1LOGIN_H

// Auto-generated from Packet1Login.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet1Login : public Packet {
public:
	Packet1Login();
	Packet1Login(const JavaString& var1, const JavaString& var2, int var3);
	virtual ~Packet1Login();

	int protocolVersion;
	JavaString username;
	JavaString password;
	long long mapSeed;
	unsigned char dimension;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET1LOGIN_H