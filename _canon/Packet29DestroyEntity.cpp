#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet29DestroyEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet29DestroyEntity.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet29DestroyEntity::~Packet29DestroyEntity()
{
}

void Packet29DestroyEntity::readPacketData(DataInputStream* var1)
{
	this->entityId = var1->readInt();
}

void Packet29DestroyEntity::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->entityId);
}

void Packet29DestroyEntity::processPacket(NetHandler* var1)
{
	var1->handleDestroyEntity(this);
}

int Packet29DestroyEntity::getPacketSize()
{
	return 4;
}

void Packet29DestroyEntity::initStatics()
{
}
