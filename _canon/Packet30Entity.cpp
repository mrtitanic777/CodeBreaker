#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet30Entity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet30Entity.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet30Entity::Packet30Entity()
	: entityId(0)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, yaw(0)
	, pitch(0)
	, rotating(0)
{
	this->rotating = false;
}

Packet30Entity::~Packet30Entity()
{
}

void Packet30Entity::readPacketData(DataInputStream* var1)
{
	this->entityId = var1->readInt();
}

void Packet30Entity::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->entityId);
}

void Packet30Entity::processPacket(NetHandler* var1)
{
	var1->handleEntity(this);
}

int Packet30Entity::getPacketSize()
{
	return 4;
}

void Packet30Entity::initStatics()
{
}
