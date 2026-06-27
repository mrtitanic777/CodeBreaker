#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet32EntityLook.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet32EntityLook.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "Packet.h"
#include "Packet30Entity.h"

Packet32EntityLook::Packet32EntityLook()
{
	this->rotating = true;
}

Packet32EntityLook::~Packet32EntityLook()
{
}

void Packet32EntityLook::readPacketData(DataInputStream* var1)
{
	Packet30Entity::readPacketData(var1);
	this->yaw = var1->readByte();
	this->pitch = var1->readByte();
}

void Packet32EntityLook::writePacketData(DataOutputStream* var1)
{
	Packet30Entity::writePacketData(var1);
	var1->writeByte(this->yaw);
	var1->writeByte(this->pitch);
}

int Packet32EntityLook::getPacketSize()
{
	return 6;
}

void Packet32EntityLook::initStatics()
{
}
