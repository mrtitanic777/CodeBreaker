#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet17AddToInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet17AddToInventory.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet17AddToInventory::~Packet17AddToInventory()
{
}

void Packet17AddToInventory::readPacketData(DataInputStream* var1)
{
	this->id = var1->readShort();
	this->count = var1->readByte();
	this->durability = var1->readShort();
}

void Packet17AddToInventory::writePacketData(DataOutputStream* var1)
{
	var1->writeShort(this->id);
	var1->writeByte(this->count);
	var1->writeShort(this->durability);
}

void Packet17AddToInventory::processPacket(NetHandler* var1)
{
	var1->handleAddToInventory(this);
}

int Packet17AddToInventory::getPacketSize()
{
	return 5;
}

void Packet17AddToInventory::initStatics()
{
}
