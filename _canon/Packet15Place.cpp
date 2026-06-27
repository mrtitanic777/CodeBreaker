#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet15Place.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet15Place.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet15Place::Packet15Place()
	: id(0)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, direction(0)
{
}

Packet15Place::Packet15Place(int var1, int var2, int var3, int var4, int var5)
	: id(0)
	, xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, direction(0)
{
	this->id = var1;
	this->xPosition = var2;
	this->yPosition = var3;
	this->zPosition = var4;
	this->direction = var5;
}

Packet15Place::~Packet15Place()
{
}

void Packet15Place::readPacketData(DataInputStream* var1)
{
	this->id = var1->readShort();
	this->xPosition = var1->readInt();
	this->yPosition = var1->read();
	this->zPosition = var1->readInt();
	this->direction = var1->read();
}

void Packet15Place::writePacketData(DataOutputStream* var1)
{
	var1->writeShort(this->id);
	var1->writeInt(this->xPosition);
	var1->write(this->yPosition);
	var1->writeInt(this->zPosition);
	var1->write(this->direction);
}

void Packet15Place::processPacket(NetHandler* var1)
{
	var1->handlePlace(this);
}

int Packet15Place::getPacketSize()
{
	return 12;
}

void Packet15Place::initStatics()
{
}
