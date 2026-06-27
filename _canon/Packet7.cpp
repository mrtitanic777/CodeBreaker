#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet7.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet7.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet7::Packet7()
	: playerEntityId(0)
	, targetEntityId(0)
	, isLeftClick(0)
{
}

Packet7::Packet7(int var1, int var2, int var3)
	: playerEntityId(0)
	, targetEntityId(0)
	, isLeftClick(0)
{
	this->playerEntityId = var1;
	this->targetEntityId = var2;
	this->isLeftClick = var3;
}

Packet7::~Packet7()
{
}

void Packet7::readPacketData(DataInputStream* var1)
{
	this->playerEntityId = var1->readInt();
	this->targetEntityId = var1->readInt();
	this->isLeftClick = var1->readByte();
}

void Packet7::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->playerEntityId);
	var1->writeInt(this->targetEntityId);
	var1->writeByte(this->isLeftClick);
}

void Packet7::processPacket(NetHandler* var1)
{
	var1->handleUseEntity(this);
}

int Packet7::getPacketSize()
{
	return 9;
}

void Packet7::initStatics()
{
}
