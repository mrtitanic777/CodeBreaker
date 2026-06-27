#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet10Flying.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet10Flying.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet10Flying::Packet10Flying()
	: xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, stance(0)
	, yaw(0)
	, pitch(0)
	, onGround(0)
	, moving(0)
	, rotating(0)
{
}

Packet10Flying::Packet10Flying(bool var1)
	: xPosition(0)
	, yPosition(0)
	, zPosition(0)
	, stance(0)
	, yaw(0)
	, pitch(0)
	, onGround(0)
	, moving(0)
	, rotating(0)
{
	this->onGround = var1;
}

Packet10Flying::~Packet10Flying()
{
}

void Packet10Flying::processPacket(NetHandler* var1)
{
	var1->handleFlying(this);
}

void Packet10Flying::readPacketData(DataInputStream* var1)
{
	this->onGround = var1->read() != 0;
}

void Packet10Flying::writePacketData(DataOutputStream* var1)
{
	var1->write(this->onGround ? 1 : 0);
}

int Packet10Flying::getPacketSize()
{
	return 1;
}

void Packet10Flying::initStatics()
{
}
