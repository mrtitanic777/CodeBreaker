#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet31RelEntityMove.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet31RelEntityMove.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "Packet.h"
#include "Packet30Entity.h"

Packet31RelEntityMove::~Packet31RelEntityMove()
{
}

void Packet31RelEntityMove::readPacketData(DataInputStream* var1)
{
	Packet30Entity::readPacketData(var1);
	this->xPosition = var1->readByte();
	this->yPosition = var1->readByte();
	this->zPosition = var1->readByte();
}

void Packet31RelEntityMove::writePacketData(DataOutputStream* var1)
{
	Packet30Entity::writePacketData(var1);
	var1->writeByte(this->xPosition);
	var1->writeByte(this->yPosition);
	var1->writeByte(this->zPosition);
}

int Packet31RelEntityMove::getPacketSize()
{
	return 7;
}

void Packet31RelEntityMove::initStatics()
{
}
