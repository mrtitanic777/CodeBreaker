#include "stdafx.h"
// Auto-generated from NoiseGeneratorOctaves2.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGeneratorOctaves2.h"
#include "Common.h"
#include "NoiseGenerator2.h"
#include "Random.h"

NoiseGeneratorOctaves2::NoiseGeneratorOctaves2()
	: m_field_4234_a(NULL)
	, m_field_4233_b(0)
{
}

NoiseGeneratorOctaves2::NoiseGeneratorOctaves2(Random* var1, int var2)
	: m_field_4234_a(NULL)
	, m_field_4233_b(0)
{
		m_field_4233_b = var2;
		m_field_4234_a = new NoiseGenerator2[var2];
		m_field_4234_a_count = var2;

		for(int var3 = 0; var3 < var2; ++var3) {
			m_field_4234_a[var3] = NoiseGenerator2(var1);
		}

}

NoiseGeneratorOctaves2::~NoiseGeneratorOctaves2() {
	delete[] m_field_4234_a;
	m_field_4234_a = NULL;
}

double* NoiseGeneratorOctaves2::func_4112_a(double* var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12) {
		return func_4111_a(var1, var2, var4, var6, var7, var8, var10, var12, 0.5f);
}

double* NoiseGeneratorOctaves2::func_4111_a(double* var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12, double var14) {
		var8 /= 1.5f;
		var10 /= 1.5f;
		int var1_count = 0;
		if(var1 != NULL && var1_count >= var6 * var7) {
			for(int var16 = 0; var16 < var1_count /* PORT: set this from allocation */; ++var16) {
				var1[var16] = 0.0f;
			}
		} else {
			var1 = new double[var6 * var7];
		}

		double var21 = 1.0f;
		double var18 = 1.0f;

		for(int var20 = 0; var20 < m_field_4233_b; ++var20) {
			m_field_4234_a[var20].func_4157_a(var1, var2, var4, var6, var7, var8 * var18, var10 * var18, 0.55f / var21);
			var18 *= var12;
			var21 *= var14;
		}

		return var1;
}
