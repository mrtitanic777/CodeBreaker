#include "stdafx.h"
// Auto-generated from NoiseGenerator2.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "NoiseGenerator2.h"
#include "Common.h"
#include "Random.h"

int** NoiseGenerator2::s_field_4296_d = NULL;
double NoiseGenerator2::s_field_4294_f = 0.5f * (sqrtf(3.0f) - 1.0f);
double NoiseGenerator2::s_field_4293_g = (3.0f - sqrtf(3.0f)) / 6.0f;

NoiseGenerator2::NoiseGenerator2()
	: m_field_4295_e(0)
	, m_field_4292_a(0)
	, m_field_4291_b(0)
	, m_field_4297_c(0)
{
  // PORT: constructor delegation  // was: this(new Random());
}

NoiseGenerator2::NoiseGenerator2(Random* var1)
	: m_field_4295_e(0)
	, m_field_4292_a(0)
	, m_field_4291_b(0)
	, m_field_4297_c(0)
{
		m_field_4295_e = new int[512];
		m_field_4295_e_count = 512;
		m_field_4292_a = var1->nextDouble() * 256.0f;
		m_field_4291_b = var1->nextDouble() * 256.0f;
		m_field_4297_c = var1->nextDouble() * 256.0f;

		int var2;
		for(var2 = 0; var2 < 256; m_field_4295_e[var2] = var2++) {
		}

		for(var2 = 0; var2 < 256; ++var2) {
			int var3 = var1->nextInt(256 - var2) + var2;
			int var4 = m_field_4295_e[var2];
			m_field_4295_e[var2] = m_field_4295_e[var3];
			m_field_4295_e[var3] = var4;
			m_field_4295_e[var2 + 256] = m_field_4295_e[var2];
		}

}

NoiseGenerator2::~NoiseGenerator2() {
	delete[] m_field_4295_e;
	m_field_4295_e = NULL;
}

int NoiseGenerator2::func_4155_a(double var0) {
		return var0 > 0.0f ? (int)var0 : (int)var0 - 1;
}

double NoiseGenerator2::func_4156_a(int* var0, double var1, double var3) {
		return (double)var0[0] * var1 + (double)var0[1] * var3;
}

void NoiseGenerator2::func_4157_a(double* var1, double var2, double var4, int var6, int var7, double var8, double var10, double var12) {
		int var14 = 0;

		for(int var15 = 0; var15 < var6; ++var15) {
			double var16 = (var2 + (double)var15) * var8 + m_field_4292_a;

			for(int var18 = 0; var18 < var7; ++var18) {
				double var19 = (var4 + (double)var18) * var10 + m_field_4291_b;
				double var27 = (var16 + var19) * s_field_4294_f;
				int var29 = func_4155_a(var16 + var27);
				int var30 = func_4155_a(var19 + var27);
				double var31 = (double)(var29 + var30) * s_field_4293_g;
				double var33 = (double)var29 - var31;
				double var35 = (double)var30 - var31;
				double var37 = var16 - var33;
				double var39 = var19 - var35;
				unsigned char var41;
				unsigned char var42;
				if(var37 > var39) {
					var41 = 1;
					var42 = 0;
				} else {
					var41 = 0;
					var42 = 1;
				}

				double var43 = var37 - (double)var41 + s_field_4293_g;
				double var45 = var39 - (double)var42 + s_field_4293_g;
				double var47 = var37 - 1.0f + 2.0f * s_field_4293_g;
				double var49 = var39 - 1.0f + 2.0f * s_field_4293_g;
				int var51 = var29 & 255;
				int var52 = var30 & 255;
				int var53 = m_field_4295_e[var51 + m_field_4295_e[var52]] % 12;
				int var54 = m_field_4295_e[var51 + var41 + m_field_4295_e[var52 + var42]] % 12;
				int var55 = m_field_4295_e[var51 + 1 + m_field_4295_e[var52 + 1]] % 12;
				double var56 = 0.5f - var37 * var37 - var39 * var39;
				double var21;
				if(var56 < 0.0f) {
					var21 = 0.0f;
				} else {
					var56 *= var56;
					var21 = var56 * var56 * func_4156_a(s_field_4296_d[var53], var37, var39);
				}

				double var58 = 0.5f - var43 * var43 - var45 * var45;
				double var23;
				if(var58 < 0.0f) {
					var23 = 0.0f;
				} else {
					var58 *= var58;
					var23 = var58 * var58 * func_4156_a(s_field_4296_d[var54], var43, var45);
				}

				double var60 = 0.5f - var47 * var47 - var49 * var49;
				double var25;
				if(var60 < 0.0f) {
					var25 = 0.0f;
				} else {
					var60 *= var60;
					var25 = var60 * var60 * func_4156_a(s_field_4296_d[var55], var47, var49);
				}

				int var10001 = var14++;
				var1[var10001] += 70.0f * (var21 + var23 + var25) * var12;
			}
		}

}
