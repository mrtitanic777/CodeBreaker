#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet12PlayerLook.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet12PlayerLook.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "Packet.h"
#include "Packet10Flying.h"

Packet12PlayerLook::Packet12PlayerLook()
{
	this->rotating = true;
}

Packet12PlayerLook::Packet12PlayerLook(float var1, float var2, bool var3)
{
	this->yaw = var1;
	this->pitch = var2;
	this->onGround = var3;
	this->rotating = true;
}

Packet12PlayerLook::~Packet12PlayerLook()
{
}

void Packet12PlayerLook::readPacketData(DataInputStream* var1)
{
	this->yaw = var1->readFloat();
	this->pitch = var1->readFloat();
	Packet10Flying::readPacketData(var1);
}

void Packet12PlayerLook::writePacketData(DataOutputStream* var1)
{
	var1->writeFloat(this->yaw);
	var1->writeFloat(this->pitch);
	Packet10Flying::writePacketData(var1);
}

int Packet12PlayerLook::getPacketSize()
{
	return 9;
}

void Packet12PlayerLook::initStatics()
{
}
