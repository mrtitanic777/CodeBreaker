#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet39.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet39.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet39::~Packet39()
{
}

int Packet39::getPacketSize()
{
	return 8;
}

void Packet39::readPacketData(DataInputStream* var1)
{
	this->entityId = var1->readInt();
	this->vehicleEntityId = var1->readInt();
}

void Packet39::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->entityId);
	var1->writeInt(this->vehicleEntityId);
}

void Packet39::processPacket(NetHandler* var1)
{
	var1->handleAttachEntity(this);
}

void Packet39::initStatics()
{
}
