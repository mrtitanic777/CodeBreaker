#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet16BlockItemSwitch.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet16BlockItemSwitch.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet16BlockItemSwitch::Packet16BlockItemSwitch()
	: unused(0)
	, id(0)
{
}

Packet16BlockItemSwitch::Packet16BlockItemSwitch(int var1, int var2)
	: unused(0)
	, id(0)
{
	this->unused = var1;
	this->id = var2;
}

Packet16BlockItemSwitch::~Packet16BlockItemSwitch()
{
}

void Packet16BlockItemSwitch::readPacketData(DataInputStream* var1)
{
	this->unused = var1->readInt();
	this->id = var1->readShort();
}

void Packet16BlockItemSwitch::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->unused);
	var1->writeShort(this->id);
}

void Packet16BlockItemSwitch::processPacket(NetHandler* var1)
{
	var1->handleBlockItemSwitch(this);
}

int Packet16BlockItemSwitch::getPacketSize()
{
	return 6;
}

void Packet16BlockItemSwitch::initStatics()
{
}
