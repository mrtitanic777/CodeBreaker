#ifndef PACKET3CHAT_H
#define PACKET3CHAT_H

// Auto-generated from Packet3Chat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet3Chat : public Packet {
public:
	Packet3Chat();
	Packet3Chat(const JavaString& var1);
	virtual ~Packet3Chat();

	JavaString message;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();

	static void initStatics();
};

#endif // PACKET3CHAT_H