#include "stdafx.h"
// Auto-generated from TexureWaterFlowFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TexureWaterFlowFX.h"
#include "Common.h"
#include "Block.h"
#include "TextureFX.h"

TexureWaterFlowFX::TexureWaterFlowFX()
	: TextureFX(Block::s_waterStill->blockIndexInTexture + 1)
	, m_field_1138_g(new float[256])
	, m_field_1137_h(new float[256])
	, m_field_1136_i(new float[256])
	, m_field_1135_j(new float[256])
	, m_field_1134_k(0)
{
		m_field_1129_e = 2;
}

TexureWaterFlowFX::~TexureWaterFlowFX() {
	delete[] m_field_1138_g;
	m_field_1138_g = NULL;
	delete[] m_field_1137_h;
	m_field_1137_h = NULL;
	delete[] m_field_1136_i;
	m_field_1136_i = NULL;
	delete[] m_field_1135_j;
	m_field_1135_j = NULL;
}

void TexureWaterFlowFX::func_783_a() {
		++m_field_1134_k;

		int var1;
		int var2;
		float var3;
		int var5;
		int var6;
		for(var1 = 0; var1 < 16; ++var1) {
			for(var2 = 0; var2 < 16; ++var2) {
				var3 = 0.0F;

				for(int var4 = var2 - 2; var4 <= var2; ++var4) {
					var5 = var1 & 15;
					var6 = var4 & 15;
					var3 += m_field_1138_g[var5 + var6 * 16];
				}

				m_field_1137_h[var1 + var2 * 16] = var3 / 3.2F + m_field_1136_i[var1 + var2 * 16] * 0.8F;
			}
		}

		for(var1 = 0; var1 < 16; ++var1) {
			for(var2 = 0; var2 < 16; ++var2) {
				m_field_1136_i[var1 + var2 * 16] += m_field_1135_j[var1 + var2 * 16] * 0.05F;
				if(m_field_1136_i[var1 + var2 * 16] < 0.0F) {
					m_field_1136_i[var1 + var2 * 16] = 0.0F;
				}

				m_field_1135_j[var1 + var2 * 16] -= 0.3F;
				if(((float)::rand() / (float)RAND_MAX) < 0.2f) {
					m_field_1135_j[var1 + var2 * 16] = 0.5F;
				}
			}
		}

		float* var12 = m_field_1137_h;
		m_field_1137_h = m_field_1138_g;
		m_field_1138_g = var12;

		for(var2 = 0; var2 < 256; ++var2) {
			var3 = m_field_1138_g[var2 - m_field_1134_k * 16 & 255];
			if(var3 > 1.0F) {
				var3 = 1.0F;
			}

			if(var3 < 0.0F) {
				var3 = 0.0F;
			}

			float var13 = var3 * var3;
			var5 = (int)(32.0F + var13 * 32.0F);
			var6 = (int)(50.0F + var13 * 64.0F);
			int var7 = 255;
			int var8 = (int)(146.0F + var13 * 50.0F);
			if(m_field_1131_c) {
				int var9 = (var5 * 30 + var6 * 59 + var7 * 11) / 100;
				int var10 = (var5 * 30 + var6 * 70) / 100;
				int var11 = (var5 * 30 + var7 * 70) / 100;
				var5 = var9;
				var6 = var10;
				var7 = var11;
			}

			m_field_1127_a[var2 * 4 + 0] = (unsigned char)var5;
			m_field_1127_a[var2 * 4 + 1] = (unsigned char)var6;
			m_field_1127_a[var2 * 4 + 2] = (unsigned char)var7;
			m_field_1127_a[var2 * 4 + 3] = (unsigned char)var8;
		}

}
