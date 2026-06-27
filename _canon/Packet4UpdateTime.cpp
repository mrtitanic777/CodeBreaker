#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet4UpdateTime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet4UpdateTime.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet4UpdateTime::~Packet4UpdateTime()
{
}

void Packet4UpdateTime::readPacketData(DataInputStream* var1)
{
	this->time = var1->readLong();
}

void Packet4UpdateTime::writePacketData(DataOutputStream* var1)
{
	var1->writeLong(this->time);
}

void Packet4UpdateTime::processPacket(NetHandler* var1)
{
	var1->handleUpdateTime(this);
}

int Packet4UpdateTime::getPacketSize()
{
	return 8;
}

void Packet4UpdateTime::initStatics()
{
}
