#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet6SpawnPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet6SpawnPosition.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet6SpawnPosition::~Packet6SpawnPosition()
{
}

void Packet6SpawnPosition::readPacketData(DataInputStream* var1)
{
	this->xPosition = var1->readInt();
	this->yPosition = var1->readInt();
	this->zPosition = var1->readInt();
}

void Packet6SpawnPosition::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->xPosition);
	var1->writeInt(this->yPosition);
	var1->writeInt(this->zPosition);
}

void Packet6SpawnPosition::processPacket(NetHandler* var1)
{
	var1->handleSpawnPosition(this);
}

int Packet6SpawnPosition::getPacketSize()
{
	return 12;
}

void Packet6SpawnPosition::initStatics()
{
}
