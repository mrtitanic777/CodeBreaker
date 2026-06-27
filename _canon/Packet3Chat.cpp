#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet3Chat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet3Chat.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet3Chat::Packet3Chat()
{
}

Packet3Chat::Packet3Chat(const JavaString& var1)
{
	this->message = var1;
}

Packet3Chat::~Packet3Chat()
{
}

void Packet3Chat::readPacketData(DataInputStream* var1)
{
	this->message = var1->readUTF();
}

void Packet3Chat::writePacketData(DataOutputStream* var1)
{
	var1->writeUTF(this->message);
}

void Packet3Chat::processPacket(NetHandler* var1)
{
	var1->handleChat(this);
}

int Packet3Chat::getPacketSize()
{
	return this->message.length();
}

void Packet3Chat::initStatics()
{
}
