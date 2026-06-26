#include "stdafx.h"
// Auto-generated from Frustrum.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Frustrum.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "ClippingHelper.h"
#include "ClippingHelperImplementation.h"

Frustrum::Frustrum()
	: m_field_593_a(ClippingHelperImplementation::func_1155_a())
	, m_field_592_b(0)
	, m_field_595_c(0)
	, m_field_594_d(0)
{
}

Frustrum::~Frustrum() {
	m_field_593_a = NULL;
}

void Frustrum::func_343_a(double var1, double var3, double var5) {
		m_field_592_b = var1;
		m_field_595_c = var3;
		m_field_594_d = var5;
}

bool Frustrum::func_344_a(double var1, double var3, double var5, double var7, double var9, double var11) {
		return m_field_593_a->func_1152_a(var1 - m_field_592_b, var3 - m_field_595_c, var5 - m_field_594_d, var7 - m_field_592_b, var9 - m_field_595_c, var11 - m_field_594_d);
}

bool Frustrum::func_342_a(AxisAlignedBB var1) {
		return func_344_a(var1.minX, var1.minY, var1.minZ, var1.maxX, var1.maxY, var1.maxZ);
}
