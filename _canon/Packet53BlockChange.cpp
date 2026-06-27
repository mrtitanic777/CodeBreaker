#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet53BlockChange.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet53BlockChange.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet53BlockChange::Packet53BlockChange()
	: xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, type(0)
	, metadata(0)
{
	this->isChunkDataPacket = true;
}

Packet53BlockChange::~Packet53BlockChange()
{
}

void Packet53BlockChange::readPacketData(DataInputStream* var1)
{
	this->xPosition = var1->readInt();
	this->yPosition = var1->read();
	this->zPosition = var1->readInt();
	this->type = var1->read();
	this->metadata = var1->read();
}

void Packet53BlockChange::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->xPosition);
	var1->write(this->yPosition);
	var1->writeInt(this->zPosition);
	var1->write(this->type);
	var1->write(this->metadata);
}

void Packet53BlockChange::processPacket(NetHandler* var1)
{
	var1->handleBlockChange(this);
}

int Packet53BlockChange::getPacketSize()
{
	return 11;
}

void Packet53BlockChange::initStatics()
{
}
