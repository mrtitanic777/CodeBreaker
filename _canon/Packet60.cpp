#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet60.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet60.h"
#include "ChunkPosition.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet60::~Packet60()
{
}

void Packet60::readPacketData(DataInputStream* var1)
{
	this->explosionX = var1->readDouble();
	this->explosionY = var1->readDouble();
	this->explosionZ = var1->readDouble();
	this->explosionSize = var1->readFloat();
	int var2 = var1->readInt();
	this->destroyedBlockPositions = std::set<void*>();
	int var3 = (int)this->explosionX;
	int var4 = (int)this->explosionY;
	int var5 = (int)this->explosionZ;
	for(int var6 = 0; var6 < var2; ++var6) {
	int var7 = var1->readByte() + var3;
	int var8 = var1->readByte() + var4;
	int var9 = var1->readByte() + var5;
	this->destroyedBlockPositions.insert(new ChunkPosition(var7, var8, var9));
	}
}

void Packet60::writePacketData(DataOutputStream* var1)
{
	var1->writeDouble(this->explosionX);
	var1->writeDouble(this->explosionY);
	var1->writeDouble(this->explosionZ);
	var1->writeFloat(this->explosionSize);
	var1->writeInt(this->destroyedBlockPositions.size());
	int var2 = (int)this->explosionX;
	int var3 = (int)this->explosionY;
	int var4 = (int)this->explosionZ;
	for(std::set<void*>::iterator var5 = this->destroyedBlockPositions.begin(); var5 != this->destroyedBlockPositions.end(); ++var5) {
	ChunkPosition* var6 = (ChunkPosition*)*var5;
	int var7 = var6->x - var2;
	int var8 = var6->y - var3;
	int var9 = var6->z - var4;
	var1->writeByte(var7);
	var1->writeByte(var8);
	var1->writeByte(var9);
	}
}

void Packet60::processPacket(NetHandler* var1)
{
	var1->handleExplosion(this);
}

int Packet60::getPacketSize()
{
	return 32 + this->destroyedBlockPositions.size() * 3;
}
