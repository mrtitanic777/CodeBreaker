#include "stdafx.h"
// Auto-generated from ChunkCoordIntPair.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkCoordIntPair.h"
#include "Common.h"

ChunkCoordIntPair::ChunkCoordIntPair()
	: m_chunkXPos(0)
	, m_chunkZPos(0)
{
}

ChunkCoordIntPair::ChunkCoordIntPair(int var1, int var2)
	: m_chunkXPos(0)
	, m_chunkZPos(0)
{
		m_chunkXPos = var1;
		m_chunkZPos = var2;
}

ChunkCoordIntPair::~ChunkCoordIntPair() {
	// No dynamic resources
}

int ChunkCoordIntPair::hashCode() {
		return m_chunkXPos << 8 | m_chunkZPos;
}

bool ChunkCoordIntPair::equals(void* /* PORT: was Object */ var1) {
		ChunkCoordIntPair* var2 = (ChunkCoordIntPair*)var1;
		return var2->m_chunkXPos == m_chunkXPos && var2->m_chunkZPos == m_chunkZPos;
}
