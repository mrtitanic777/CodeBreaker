#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ChunkCoordIntPair.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkCoordIntPair.h"

ChunkCoordIntPair::ChunkCoordIntPair(int var1, int var2)
	: chunkXPos(0)
	, chunkZPos(0)
{
	this->chunkXPos = var1;
	this->chunkZPos = var2;
}

ChunkCoordIntPair::~ChunkCoordIntPair()
{
}

int ChunkCoordIntPair::hashCode()
{
	return (this->chunkXPos << 8) | this->chunkZPos;
}

bool ChunkCoordIntPair::equals(JObject var1)
{
	ChunkCoordIntPair* var2 = (ChunkCoordIntPair*)var1;
	return var2->chunkXPos == this->chunkXPos && var2->chunkZPos == this->chunkZPos;
}

void ChunkCoordIntPair::initStatics()
{
}
