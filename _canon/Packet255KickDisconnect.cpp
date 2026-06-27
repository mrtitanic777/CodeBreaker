#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet255KickDisconnect.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet255KickDisconnect.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet255KickDisconnect::Packet255KickDisconnect()
{
}

Packet255KickDisconnect::Packet255KickDisconnect(const JavaString& var1)
{
	this->reason = var1;
}

Packet255KickDisconnect::~Packet255KickDisconnect()
{
}

void Packet255KickDisconnect::readPacketData(DataInputStream* var1)
{
	this->reason = var1->readUTF();
}

void Packet255KickDisconnect::writePacketData(DataOutputStream* var1)
{
	var1->writeUTF(this->reason);
}

void Packet255KickDisconnect::processPacket(NetHandler* var1)
{
	var1->handleKickDisconnect(this);
}

int Packet255KickDisconnect::getPacketSize()
{
	return this->reason.length();
}

void Packet255KickDisconnect::initStatics()
{
}
