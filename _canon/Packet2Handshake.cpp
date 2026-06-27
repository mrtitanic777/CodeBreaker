#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet2Handshake.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet2Handshake.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet2Handshake::Packet2Handshake()
{
}

Packet2Handshake::Packet2Handshake(const JavaString& var1)
{
	this->username = var1;
}

Packet2Handshake::~Packet2Handshake()
{
}

void Packet2Handshake::readPacketData(DataInputStream* var1)
{
	this->username = var1->readUTF();
}

void Packet2Handshake::writePacketData(DataOutputStream* var1)
{
	var1->writeUTF(this->username);
}

void Packet2Handshake::processPacket(NetHandler* var1)
{
	var1->handleHandshake(this);
}

int Packet2Handshake::getPacketSize()
{
	return 4 + this->username.length() + 4;
}

void Packet2Handshake::initStatics()
{
}
