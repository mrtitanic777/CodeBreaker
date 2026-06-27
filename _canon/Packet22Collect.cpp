#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet22Collect.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet22Collect.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet22Collect::~Packet22Collect()
{
}

void Packet22Collect::readPacketData(DataInputStream* var1)
{
	this->collectedEntityId = var1->readInt();
	this->collectorEntityId = var1->readInt();
}

void Packet22Collect::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->collectedEntityId);
	var1->writeInt(this->collectorEntityId);
}

void Packet22Collect::processPacket(NetHandler* var1)
{
	var1->handleCollect(this);
}

int Packet22Collect::getPacketSize()
{
	return 8;
}

void Packet22Collect::initStatics()
{
}
