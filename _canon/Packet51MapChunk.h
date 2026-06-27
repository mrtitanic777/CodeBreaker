#ifndef PACKET51MAPCHUNK_H
#define PACKET51MAPCHUNK_H

// Auto-generated from Packet51MapChunk.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Packet.h"

class DataInputStream;
class DataOutputStream;
class NetHandler;

class Packet51MapChunk : public Packet {
public:
	Packet51MapChunk();
	virtual ~Packet51MapChunk();

	int xPosition;
	int yPosition;
	int zPosition;
	int xSize;
	int ySize;
	int zSize;
	JArray<unsigned char > chunk;
	int chunkSize;

	virtual void readPacketData(DataInputStream* var1);
	virtual void writePacketData(DataOutputStream* var1);
	virtual void processPacket(NetHandler* var1);
	virtual int getPacketSize();
};

#endif // PACKET51MAPCHUNK_H