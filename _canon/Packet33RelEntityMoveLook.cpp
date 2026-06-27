#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet33RelEntityMoveLook.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet33RelEntityMoveLook.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "Packet.h"
#include "Packet30Entity.h"

Packet33RelEntityMoveLook::Packet33RelEntityMoveLook()
{
	this->rotating = true;
}

Packet33RelEntityMoveLook::~Packet33RelEntityMoveLook()
{
}

void Packet33RelEntityMoveLook::readPacketData(DataInputStream* var1)
{
	Packet30Entity::readPacketData(var1);
	this->xPosition = var1->readByte();
	this->yPosition = var1->readByte();
	this->zPosition = var1->readByte();
	this->yaw = var1->readByte();
	this->pitch = var1->readByte();
}

void Packet33RelEntityMoveLook::writePacketData(DataOutputStream* var1)
{
	Packet30Entity::writePacketData(var1);
	var1->writeByte(this->xPosition);
	var1->writeByte(this->yPosition);
	var1->writeByte(this->zPosition);
	var1->writeByte(this->yaw);
	var1->writeByte(this->pitch);
}

int Packet33RelEntityMoveLook::getPacketSize()
{
	return 9;
}

void Packet33RelEntityMoveLook::initStatics()
{
}
