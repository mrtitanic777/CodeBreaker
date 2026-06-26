#include "stdafx.h"
// Auto-generated from TexturePortalFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TexturePortalFX.h"
#include "Common.h"
#include "Random.h"
#include "TextureFX.h"
#include "Block.h"
#include "BlockPortal.h"

TexturePortalFX::TexturePortalFX()
	: TextureFX(Block::s_portal->blockIndexInTexture)
	, m_field_4227_g(0)
	, m_field_4226_h(NULL /* PORT: allocate byte[32][1024] */)
{
		Random* var1 = new Random(100L);

		for(int var2 = 0; var2 < 32; ++var2) {
			for(int var3 = 0; var3 < 16; ++var3) {
				for(int var4 = 0; var4 < 16; ++var4) {
					float var5 = 0.0F;

					int var6;
					for(var6 = 0; var6 < 2; ++var6) {
						float var7 = (float)(var6 * 8);
						float var8 = (float)(var6 * 8);
						float var9 = ((float)var3 - var7) / 16.0F * 2.0F;
						float var10 = ((float)var4 - var8) / 16.0F * 2.0F;
						if(var9 < -1.0F) {
							var9 += 2.0F;
						}

						if(var9 >= 1.0F) {
							var9 -= 2.0F;
						}

						if(var10 < -1.0F) {
							var10 += 2.0F;
						}

						if(var10 >= 1.0F) {
							var10 -= 2.0F;
						}

						float var11 = var9 * var9 + var10 * var10;
						float var12 = (float)atan2f((double)var10, (double)var9) + ((float)var2 / 32.0F * 3.14159265358979323846f * 2.0F - var11 * 10.0F + (float)(var6 * 2)) * (float)(var6 * 2 - 1);
						var12 = (sinf(var12) + 1.0F) / 2.0F;
						var12 /= var11 + 1.0F;
						var5 += var12 * 0.5F;
					}

					var5 += var1->nextFloat() * 0.1F;
					var6 = (int)(var5 * 100.0F + 155.0F);
					int var13 = (int)(var5 * var5 * 200.0F + 55.0F);
					int var14 = (int)(var5 * var5 * var5 * var5 * 255.0F);
					int var15 = (int)(var5 * 100.0F + 155.0F);
					int var16 = var4 * 16 + var3;
					m_field_4226_h[var2][var16 * 4 + 0] = (unsigned char)var13;
					m_field_4226_h[var2][var16 * 4 + 1] = (unsigned char)var14;
					m_field_4226_h[var2][var16 * 4 + 2] = (unsigned char)var6;
					m_field_4226_h[var2][var16 * 4 + 3] = (unsigned char)var15;
				}
			}
		}

}

TexturePortalFX::~TexturePortalFX() {
	delete[] m_field_4226_h;
	m_field_4226_h = NULL;
}

void TexturePortalFX::func_783_a() {
		++m_field_4227_g;
		unsigned char* var1 = m_field_4226_h[m_field_4227_g & 31];

		for(int var2 = 0; var2 < 256; ++var2) {
			int var3 = var1[var2 * 4 + 0] & 255;
			int var4 = var1[var2 * 4 + 1] & 255;
			int var5 = var1[var2 * 4 + 2] & 255;
			int var6 = var1[var2 * 4 + 3] & 255;
			if(m_field_1131_c) {
				int var7 = (var3 * 30 + var4 * 59 + var5 * 11) / 100;
				int var8 = (var3 * 30 + var4 * 70) / 100;
				int var9 = (var3 * 30 + var5 * 70) / 100;
				var3 = var7;
				var4 = var8;
				var5 = var9;
			}

			m_field_1127_a[var2 * 4 + 0] = (unsigned char)var3;
			m_field_1127_a[var2 * 4 + 1] = (unsigned char)var4;
			m_field_1127_a[var2 * 4 + 2] = (unsigned char)var5;
			m_field_1127_a[var2 * 4 + 3] = (unsigned char)var6;
		}

}
