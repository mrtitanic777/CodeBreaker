#include "stdafx.h"
// Auto-generated from ChunkPosition.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkPosition.h"
#include "Common.h"

ChunkPosition::ChunkPosition()
	: m_x(0)
	, m_y(0)
	, m_z(0)
{
}

ChunkPosition::ChunkPosition(int var1, int var2, int var3)
	: m_x(0)
	, m_y(0)
	, m_z(0)
{
		m_x = var1;
		m_y = var2;
		m_z = var3;
}

ChunkPosition::~ChunkPosition() {
	// No dynamic resources
}

bool ChunkPosition::equals(void* /* PORT: was Object */ var1) {
		// PORT: Java did `var1 instanceof ChunkPosition`. The param is void* (RTTI
		// can't be applied to void*), so guard NULL and reinterpret — every caller
		// passes a ChunkPosition*.
		if(var1 == NULL) {
			return false;
		} else {
			ChunkPosition* var2 = (ChunkPosition*)var1;
			return var2->m_x == m_x && var2->m_y == m_y && var2->m_z == m_z;
		}
}

int ChunkPosition::hashCode() {
		return m_x * 8976890 + m_y * 981131 + m_z;
}
