#ifndef PACKET_H
#define PACKET_H

// Auto-generated from Packet.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>

class Class;
class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet {
public:
	Packet();   // synthesized: Java field initializers (override had no ctor)
	virtual ~Packet();

	bool isChunkDataPacket;
	// Java: Map<Integer,Class> id->class and Map<Class,Integer> class->id.
	// Class identity is keyed by its runtime type name (typeid) string.
	static std::map<int, void*> s_packetIdToClassMap;
	static std::map<std::string, int> s_packetClassToIdMap;

	static void initStatics();   // run the packet registrations in order (called by j2x_initAll)
	static void addIdClassMapping(int var0, Class* var1);
	static Packet* getNewPacket(int var0);
	virtual int getPacketId();
	static Packet* readPacket(DataInputStream* var0);
	static void writePacket(Packet* var0, DataOutputStream* var1);
	virtual void readPacketData(DataInputStream* var1) = 0;
	virtual void writePacketData(DataOutputStream* var1) = 0;
	virtual void processPacket(NetHandler* var1) = 0;
	virtual int getPacketSize() = 0;
};

#endif // PACKET_H