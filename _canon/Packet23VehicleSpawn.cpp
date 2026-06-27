#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from Packet23VehicleSpawn.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Packet23VehicleSpawn.h"
#include "DataInputStream.h"
#include "DataOutputStream.h"
#include "NetHandler.h"
#include "Packet.h"

Packet23VehicleSpawn::~Packet23VehicleSpawn()
{
}

void Packet23VehicleSpawn::readPacketData(DataInputStream* var1)
{
	this->entityId = var1->readInt();
	this->type = var1->readByte();
	this->xPosition = var1->readInt();
	this->yPosition = var1->readInt();
	this->zPosition = var1->readInt();
}

void Packet23VehicleSpawn::writePacketData(DataOutputStream* var1)
{
	var1->writeInt(this->entityId);
	var1->writeByte(this->type);
	var1->writeInt(this->xPosition);
	var1->writeInt(this->yPosition);
	var1->writeInt(this->zPosition);
}

void Packet23VehicleSpawn::processPacket(NetHandler* var1)
{
	var1->handleVehicleSpawn(this);
}

int Packet23VehicleSpawn::getPacketSize()
{
	return 17;
}

void Packet23VehicleSpawn::initStatics()
{
}
