#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ChunkCoordinates.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkCoordinates.h"

ChunkCoordinates::ChunkCoordinates(int var1, int var2)
	: posX(0)
	, posZ(0)
{
	this->posX = var1;
	this->posZ = var2;
}

ChunkCoordinates::~ChunkCoordinates()
{
}

bool ChunkCoordinates::equals(JObject var1)
{
	if(!(((var1).is<ChunkCoordinates>()))) {
	return false;
	} else {
	ChunkCoordinates* var2 = (ChunkCoordinates*)var1;
	return this->posX == var2->posX && this->posZ == var2->posZ;
	}
}

int ChunkCoordinates::hashCode()
{
	return (this->posX << 16) ^ this->posZ;
}

void ChunkCoordinates::initStatics()
{
}
