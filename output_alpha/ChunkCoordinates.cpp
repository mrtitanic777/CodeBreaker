#include "stdafx.h"
// Auto-generated from ChunkCoordinates.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkCoordinates.h"
#include "Common.h"

ChunkCoordinates::ChunkCoordinates()
	: m_field_1518_a(0)
	, m_field_1517_b(0)
{
}

ChunkCoordinates::ChunkCoordinates(int var1, int var2)
	: m_field_1518_a(0)
	, m_field_1517_b(0)
{
		m_field_1518_a = var1;
		m_field_1517_b = var2;
}

ChunkCoordinates::~ChunkCoordinates() {
	// No dynamic resources
}

bool ChunkCoordinates::equals(void* /* PORT: was Object */ var1) {
		// PORT: Java did `var1 instanceof ChunkCoordinates`. The param is void*
		// (RTTI can't be applied to void*), so guard NULL and reinterpret — every
		// caller passes a ChunkCoordinates*.
		if(var1 == NULL) {
			return false;
		} else {
			ChunkCoordinates* var2 = (ChunkCoordinates*)var1;
			return m_field_1518_a == var2->m_field_1518_a && m_field_1517_b == var2->m_field_1517_b;
		}
}

int ChunkCoordinates::hashCode() {
		return m_field_1518_a << 16 ^ m_field_1517_b;
}
