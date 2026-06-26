#include "stdafx.h"
// Auto-generated from ClippingHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ClippingHelper.h"
#include "Common.h"

ClippingHelper::ClippingHelper()
	: m_field_1688_a(NULL /* PORT: allocate float[16][16] */)
	, m_field_1687_b(new float[16])
	, m_field_1690_c(new float[16])
	, m_field_1689_d(new float[16])
{
}

ClippingHelper::~ClippingHelper() {
	delete[] m_field_1688_a;
	m_field_1688_a = NULL;
	delete[] m_field_1687_b;
	m_field_1687_b = NULL;
	delete[] m_field_1690_c;
	m_field_1690_c = NULL;
	delete[] m_field_1689_d;
	m_field_1689_d = NULL;
}

bool ClippingHelper::func_1152_a(double var1, double var3, double var5, double var7, double var9, double var11) {
		for(int var13 = 0; var13 < 6; ++var13) {
			if((double)m_field_1688_a[var13][0] * var1 + (double)m_field_1688_a[var13][1] * var3 + (double)m_field_1688_a[var13][2] * var5 + (double)m_field_1688_a[var13][3] <= 0.0f && (double)m_field_1688_a[var13][0] * var7 + (double)m_field_1688_a[var13][1] * var3 + (double)m_field_1688_a[var13][2] * var5 + (double)m_field_1688_a[var13][3] <= 0.0f && (double)m_field_1688_a[var13][0] * var1 + (double)m_field_1688_a[var13][1] * var9 + (double)m_field_1688_a[var13][2] * var5 + (double)m_field_1688_a[var13][3] <= 0.0f && (double)m_field_1688_a[var13][0] * var7 + (double)m_field_1688_a[var13][1] * var9 + (double)m_field_1688_a[var13][2] * var5 + (double)m_field_1688_a[var13][3] <= 0.0f && (double)m_field_1688_a[var13][0] * var1 + (double)m_field_1688_a[var13][1] * var3 + (double)m_field_1688_a[var13][2] * var11 + (double)m_field_1688_a[var13][3] <= 0.0f && (double)m_field_1688_a[var13][0] * var7 + (double)m_field_1688_a[var13][1] * var3 + (double)m_field_1688_a[var13][2] * var11 + (double)m_field_1688_a[var13][3] <= 0.0f && (double)m_field_1688_a[var13][0] * var1 + (double)m_field_1688_a[var13][1] * var9 + (double)m_field_1688_a[var13][2] * var11 + (double)m_field_1688_a[var13][3] <= 0.0f && (double)m_field_1688_a[var13][0] * var7 + (double)m_field_1688_a[var13][1] * var9 + (double)m_field_1688_a[var13][2] * var11 + (double)m_field_1688_a[var13][3] <= 0.0f) {
				return false;
			}
		}

		return true;
}
