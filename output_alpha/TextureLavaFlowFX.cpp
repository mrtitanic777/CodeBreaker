#include "stdafx.h"
// Auto-generated from TextureLavaFlowFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureLavaFlowFX.h"
#include "Common.h"
#include "Block.h"
#include "TextureFX.h"

TextureLavaFlowFX::TextureLavaFlowFX()
	: TextureFX(Block::s_lavaStill->blockIndexInTexture + 1)
	, m_field_1143_g(new float[256])
	, m_field_1142_h(new float[256])
	, m_field_1141_i(new float[256])
	, m_field_1140_j(new float[256])
	, m_field_1139_k(0)
{
		m_field_1129_e = 2;
}

TextureLavaFlowFX::~TextureLavaFlowFX() {
	delete[] m_field_1143_g;
	m_field_1143_g = NULL;
	delete[] m_field_1142_h;
	m_field_1142_h = NULL;
	delete[] m_field_1141_i;
	m_field_1141_i = NULL;
	delete[] m_field_1140_j;
	m_field_1140_j = NULL;
}

void TextureLavaFlowFX::func_783_a() {
		++m_field_1139_k;

		int var2;
		float var3;
		int var5;
		int var6;
		int var7;
		int var8;
		int var9;
		for(int var1 = 0; var1 < 16; ++var1) {
			for(var2 = 0; var2 < 16; ++var2) {
				var3 = 0.0F;
				int var4 = (int)(sinf((float)var2 * 3.14159265358979323846f * 2.0F / 16.0F) * 1.2F);
				var5 = (int)(sinf((float)var1 * 3.14159265358979323846f * 2.0F / 16.0F) * 1.2F);

				for(var6 = var1 - 1; var6 <= var1 + 1; ++var6) {
					for(var7 = var2 - 1; var7 <= var2 + 1; ++var7) {
						var8 = var6 + var4 & 15;
						var9 = var7 + var5 & 15;
						var3 += m_field_1143_g[var8 + var9 * 16];
					}
				}

				m_field_1142_h[var1 + var2 * 16] = var3 / 10.0F + (m_field_1141_i[(var1 + 0 & 15) + (var2 + 0 & 15) * 16] + m_field_1141_i[(var1 + 1 & 15) + (var2 + 0 & 15) * 16] + m_field_1141_i[(var1 + 1 & 15) + (var2 + 1 & 15) * 16] + m_field_1141_i[(var1 + 0 & 15) + (var2 + 1 & 15) * 16]) / 4.0F * 0.8F;
				m_field_1141_i[var1 + var2 * 16] += m_field_1140_j[var1 + var2 * 16] * 0.01F;
				if(m_field_1141_i[var1 + var2 * 16] < 0.0F) {
					m_field_1141_i[var1 + var2 * 16] = 0.0F;
				}

				m_field_1140_j[var1 + var2 * 16] -= 0.06F;
				if(((float)::rand() / (float)RAND_MAX) < 0.005f) {
					m_field_1140_j[var1 + var2 * 16] = 1.5F;
				}
			}
		}

		float* var11 = m_field_1142_h;
		m_field_1142_h = m_field_1143_g;
		m_field_1143_g = var11;

		for(var2 = 0; var2 < 256; ++var2) {
			var3 = m_field_1143_g[var2 - m_field_1139_k / 3 * 16 & 255] * 2.0F;
			if(var3 > 1.0F) {
				var3 = 1.0F;
			}

			if(var3 < 0.0F) {
				var3 = 0.0F;
			}

			var5 = (int)(var3 * 100.0F + 155.0F);
			var6 = (int)(var3 * var3 * 255.0F);
			var7 = (int)(var3 * var3 * var3 * var3 * 128.0F);
			if(m_field_1131_c) {
				var8 = (var5 * 30 + var6 * 59 + var7 * 11) / 100;
				var9 = (var5 * 30 + var6 * 70) / 100;
				int var10 = (var5 * 30 + var7 * 70) / 100;
				var5 = var8;
				var6 = var9;
				var7 = var10;
			}

			m_field_1127_a[var2 * 4 + 0] = (unsigned char)var5;
			m_field_1127_a[var2 * 4 + 1] = (unsigned char)var6;
			m_field_1127_a[var2 * 4 + 2] = (unsigned char)var7;
			m_field_1127_a[var2 * 4 + 3] = -1;
		}

}
