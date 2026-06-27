#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ChunkPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkPosition.h"

ChunkPosition::ChunkPosition(int var1, int var2, int var3)
	: x(0)
	, y(0)
	, z(0)
{
	this->x = var1;
	this->y = var2;
	this->z = var3;
}

ChunkPosition::~ChunkPosition()
{
}

bool ChunkPosition::equals(JObject var1)
{
	if(!(((var1).is<ChunkPosition>()))) {
	return false;
	} else {
	ChunkPosition* var2 = (ChunkPosition*)var1;
	return var2->x == this->x && var2->y == this->y && var2->z == this->z;
	}
}

int ChunkPosition::hashCode()
{
	return this->x * 8976890 + this->y * 981131 + this->z;
}

void ChunkPosition::initStatics()
{
}
