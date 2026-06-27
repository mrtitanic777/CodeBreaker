#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet13PlayerLookMove.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet13PlayerLookMove.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "Packet.h"
#include "Packet10Flying.h"

Packet13PlayerLookMove::Packet13PlayerLookMove()
{
	this->rotating = true;
	this->moving = true;
}

Packet13PlayerLookMove::Packet13PlayerLookMove(double var1, double var3, double var5, double var7, float var9, float var10, bool var11)
{
	this->xPosition = var1;
	this->yPosition = var3;
	this->stance = var5;
	this->zPosition = var7;
	this->yaw = var9;
	this->pitch = var10;
	this->onGround = var11;
	this->rotating = true;
	this->moving = true;
}

Packet13PlayerLookMove::~Packet13PlayerLookMove()
{
}

void Packet13PlayerLookMove::readPacketData(DataInputStream* var1)
{
	this->xPosition = var1->readDouble();
	this->yPosition = var1->readDouble();
	this->stance = var1->readDouble();
	this->zPosition = var1->readDouble();
	this->yaw = var1->readFloat();
	this->pitch = var1->readFloat();
	Packet10Flying::readPacketData(var1);
}

void Packet13PlayerLookMove::writePacketData(DataOutputStream* var1)
{
	var1->writeDouble(this->xPosition);
	var1->writeDouble(this->yPosition);
	var1->writeDouble(this->stance);
	var1->writeDouble(this->zPosition);
	var1->writeFloat(this->yaw);
	var1->writeFloat(this->pitch);
	Packet10Flying::writePacketData(var1);
}

int Packet13PlayerLookMove::getPacketSize()
{
	return 41;
}

void Packet13PlayerLookMove::initStatics()
{
}
