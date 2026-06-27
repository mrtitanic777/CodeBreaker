#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet50PreChunk.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet50PreChunk.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet50PreChunk::~Packet50PreChunk()
{
}

void Packet50PreChunk::readPacketData(DataInputStream* var1)
{
	this->xPosition = var1->readInt();
	this->yPosition = var1->readInt();
	this->mode = var1->read() != 0;
}

void Packet50PreChunk::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->xPosition);
	var1->writeInt(this->yPosition);
	var1->write(this->mode ? 1 : 0);
}

void Packet50PreChunk::processPacket(NetHandler* var1)
{
	var1->handlePreChunk(this);
}

int Packet50PreChunk::getPacketSize()
{
	return 9;
}

void Packet50PreChunk::initStatics()
{
}
