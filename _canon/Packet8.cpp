#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet8.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet8.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet8::~Packet8()
{
}

void Packet8::readPacketData(DataInputStream* var1)
{
	this->healthMP = var1->readByte();
}

void Packet8::writePacketData(DataOutputStream* var1)
{
	var1->writeByte(this->healthMP);
}

void Packet8::processPacket(NetHandler* var1)
{
	var1->handleHealth(this);
}

int Packet8::getPacketSize()
{
	return 1;
}

void Packet8::initStatics()
{
}
