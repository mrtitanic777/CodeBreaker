#include "stdafx.h"
// Auto-generated from TextureFlamesFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFlamesFX.h"
#include "Common.h"
#include "Block.h"
#include "TextureFX.h"
#include "BlockFire.h"

TextureFlamesFX::TextureFlamesFX()
	: m_field_1133_g(new float[320])
	, m_field_1132_h(new float[320])
{
}

TextureFlamesFX::TextureFlamesFX(int var1)
	: TextureFX(Block::s_fire->blockIndexInTexture + var1 * 16)
	, m_field_1133_g(new float[320])
	, m_field_1132_h(new float[320])
{
}

TextureFlamesFX::~TextureFlamesFX() {
	delete[] m_field_1133_g;
	m_field_1133_g = NULL;
	delete[] m_field_1132_h;
	m_field_1132_h = NULL;
}

void TextureFlamesFX::func_783_a() {
		int var2;
		float var4;
		int var5;
		int var6;
		for(int var1 = 0; var1 < 16; ++var1) {
			for(var2 = 0; var2 < 20; ++var2) {
				int var3 = 18;
				var4 = m_field_1133_g[var1 + (var2 + 1) % 20 * 16] * (float)var3;

				for(var5 = var1 - 1; var5 <= var1 + 1; ++var5) {
					for(var6 = var2; var6 <= var2 + 1; ++var6) {
						if(var5 >= 0 && var6 >= 0 && var5 < 16 && var6 < 20) {
							var4 += m_field_1133_g[var5 + var6 * 16];
						}

						++var3;
					}
				}

				m_field_1132_h[var1 + var2 * 16] = var4 / ((float)var3 * 1.06F);
				if(var2 >= 19) {
					m_field_1132_h[var1 + var2 * 16] = (float)(((float)::rand() / (float)RAND_MAX) * ((float)::rand() / (float)RAND_MAX) * ((float)::rand() / (float)RAND_MAX) * 4.0f + ((float)::rand() / (float)RAND_MAX) * (double)0.1F + (double)0.2F);
				}
			}
		}

		float* var12 = m_field_1132_h;
		m_field_1132_h = m_field_1133_g;
		m_field_1133_g = var12;

		for(var2 = 0; var2 < 256; ++var2) {
			float var13 = m_field_1133_g[var2] * 1.8F;
			if(var13 > 1.0F) {
				var13 = 1.0F;
			}

			if(var13 < 0.0F) {
				var13 = 0.0F;
			}

			var5 = (int)(var13 * 155.0F + 100.0F);
			var6 = (int)(var13 * var13 * 255.0F);
			int var7 = (int)(var13 * var13 * var13 * var13 * var13 * var13 * var13 * var13 * var13 * var13 * 255.0F);
			short var8 = 255;
			if(var13 < 0.5F) {
				var8 = 0;
			}

			var4 = (var13 - 0.5F) * 2.0F;
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
