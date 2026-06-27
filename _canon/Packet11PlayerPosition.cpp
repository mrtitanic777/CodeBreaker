#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet11PlayerPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet11PlayerPosition.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "Packet.h"
#include "Packet10Flying.h"

Packet11PlayerPosition::Packet11PlayerPosition()
{
	this->moving = true;
}

Packet11PlayerPosition::Packet11PlayerPosition(double var1, double var3, double var5, double var7, bool var9)
{
	this->xPosition = var1;
	this->yPosition = var3;
	this->stance = var5;
	this->zPosition = var7;
	this->onGround = var9;
	this->moving = true;
}

Packet11PlayerPosition::~Packet11PlayerPosition()
{
}

void Packet11PlayerPosition::readPacketData(DataInputStream* var1)
{
	this->xPosition = var1->readDouble();
	this->yPosition = var1->readDouble();
	this->stance = var1->readDouble();
	this->zPosition = var1->readDouble();
	Packet10Flying::readPacketData(var1);
}

void Packet11PlayerPosition::writePacketData(DataOutputStream* var1)
{
	var1->writeDouble(this->xPosition);
	var1->writeDouble(this->yPosition);
	var1->writeDouble(this->stance);
	var1->writeDouble(this->zPosition);
	Packet10Flying::writePacketData(var1);
}

int Packet11PlayerPosition::getPacketSize()
{
	return 33;
}

void Packet11PlayerPosition::initStatics()
{
}
