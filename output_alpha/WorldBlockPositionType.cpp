#include "stdafx.h"
// Auto-generated from WorldBlockPositionType.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldBlockPositionType.h"
#include "Common.h"
#include "WorldClient.h"

WorldBlockPositionType::WorldBlockPositionType()
	: m_field_1202_a(0)
	, m_field_1201_b(0)
	, m_field_1207_c(0)
	, m_field_1206_d(0)
	, m_field_1205_e(0)
	, m_field_1204_f(0)
	, m_field_1203_g(NULL)
{
}

WorldBlockPositionType::WorldBlockPositionType(WorldClient* var1, int var2, int var3, int var4, int var5, int var6)
	: m_field_1202_a(0)
	, m_field_1201_b(0)
	, m_field_1207_c(0)
	, m_field_1206_d(0)
	, m_field_1205_e(0)
	, m_field_1204_f(0)
	, m_field_1203_g(NULL)
{
		m_field_1203_g = var1;
		m_field_1202_a = var2;
		m_field_1201_b = var3;
		m_field_1207_c = var4;
		m_field_1206_d = 80;
		m_field_1205_e = var5;
		m_field_1204_f = var6;
}

WorldBlockPositionType::~WorldBlockPositionType() {
	m_field_1203_g = NULL;
}
