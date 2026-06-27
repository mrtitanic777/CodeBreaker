#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet38.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet38.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet38::~Packet38()
{
}

void Packet38::readPacketData(DataInputStream* var1)
{
	this->entityId = var1->readInt();
	this->entityStatus = var1->readByte();
}

void Packet38::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->entityId);
	var1->writeByte(this->entityStatus);
}

void Packet38::processPacket(NetHandler* var1)
{
	var1->handleEntityStatus(this);
}

int Packet38::getPacketSize()
{
	return 5;
}

void Packet38::initStatics()
{
}
