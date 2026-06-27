#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet9.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet9.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet9::~Packet9()
{
}

void Packet9::processPacket(NetHandler* var1)
{
	var1->handleRespawn(this);
}

void Packet9::readPacketData(DataInputStream* var1)
{
}

void Packet9::writePacketData(DataOutputStream* var1)
{
}

int Packet9::getPacketSize()
{
	return 0;
}

void Packet9::initStatics()
{
}
