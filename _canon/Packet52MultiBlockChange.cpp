#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet52MultiBlockChange.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet52MultiBlockChange.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet52MultiBlockChange::Packet52MultiBlockChange()
	: xPosition(0)
	, zPosition(0)
	, size(0)
{
	this->isChunkDataPacket = true;
}

Packet52MultiBlockChange::~Packet52MultiBlockChange()
{
}

void Packet52MultiBlockChange::readPacketData(DataInputStream* var1)
{
	this->xPosition = var1->readInt();
	this->zPosition = var1->readInt();
	this->size = var1->readShort() & 0xffff;
	this->coordinateArray = JArray<short >(this->size);
	this->typeArray = JArray<unsigned char >(this->size);
	this->metadataArray = JArray<unsigned char >(this->size);
	for(int var2 = 0; var2 < this->size; ++var2) {
	this->coordinateArray[var2] = var1->readShort();
	}
	var1->readFully(this->typeArray);
	var1->readFully(this->metadataArray);
}

void Packet52MultiBlockChange::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->xPosition);
	var1->writeInt(this->zPosition);
	var1->writeShort((short)this->size);
	for(int var2 = 0; var2 < this->size; ++var2) {
	var1->writeShort(this->coordinateArray[var2]);
	}
	var1->write(this->typeArray);
	var1->write(this->metadataArray);
}

void Packet52MultiBlockChange::processPacket(NetHandler* var1)
{
	var1->handleMultiBlockChange(this);
}

int Packet52MultiBlockChange::getPacketSize()
{
	return 10 + this->size * 4;
}

void Packet52MultiBlockChange::initStatics()
{
}
