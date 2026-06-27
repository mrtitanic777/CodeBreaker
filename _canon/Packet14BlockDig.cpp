#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet14BlockDig.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet14BlockDig.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet14BlockDig::Packet14BlockDig()
	: xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, face(0)
	, status(0)
{
}

Packet14BlockDig::Packet14BlockDig(int var1, int var2, int var3, int var4, int var5)
	: xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, face(0)
	, status(0)
{
	this->status = var1;
	this->xPosition = var2;
	this->yPosition = var3;
	this->zPosition = var4;
	this->face = var5;
}

Packet14BlockDig::~Packet14BlockDig()
{
}

void Packet14BlockDig::readPacketData(DataInputStream* var1)
{
	this->status = var1->read();
	this->xPosition = var1->readInt();
	this->yPosition = var1->read();
	this->zPosition = var1->readInt();
	this->face = var1->read();
}

void Packet14BlockDig::writePacketData(DataOutputStream* var1)
{
	var1->write(this->status);
	var1->writeInt(this->xPosition);
	var1->write(this->yPosition);
	var1->writeInt(this->zPosition);
	var1->write(this->face);
}

void Packet14BlockDig::processPacket(NetHandler* var1)
{
	var1->handleBlockDig(this);
}

int Packet14BlockDig::getPacketSize()
{
	return 11;
}

void Packet14BlockDig::initStatics()
{
}
