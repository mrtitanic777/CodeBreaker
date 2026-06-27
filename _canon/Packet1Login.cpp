#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet1Login.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet1Login.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet1Login::Packet1Login()
	: protocolVersion(0)
	, mapSeed(0)
	, dimension(0)
{
}

Packet1Login::Packet1Login(const JavaString& var1, const JavaString& var2, int var3)
	: protocolVersion(0)
	, mapSeed(0)
	, dimension(0)
{
	this->username = var1;
	this->password = var2;
	this->protocolVersion = var3;
}

Packet1Login::~Packet1Login()
{
}

void Packet1Login::readPacketData(DataInputStream* var1)
{
	this->protocolVersion = var1->readInt();
	this->username = var1->readUTF();
	this->password = var1->readUTF();
	this->mapSeed = var1->readLong();
	this->dimension = var1->readByte();
}

void Packet1Login::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->protocolVersion);
	var1->writeUTF(this->username);
	var1->writeUTF(this->password);
	var1->writeLong(this->mapSeed);
	var1->writeByte(this->dimension);
}

void Packet1Login::processPacket(NetHandler* var1)
{
	var1->handleLogin(this);
}

int Packet1Login::getPacketSize()
{
	return 4 + this->username.length() + this->password.length() + 4 + 5;
}

void Packet1Login::initStatics()
{
}
