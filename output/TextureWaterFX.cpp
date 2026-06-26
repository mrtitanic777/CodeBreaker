#include "stdafx.h"
// Auto-generated from TextureWaterFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureWaterFX.h"
#include "Common.h"
#include "Block.h"
#include "TextureFX.h"

TextureWaterFX::TextureWaterFX()
	: TextureFX(Block::s_waterMoving->blockIndexInTexture)
	, m_red(new float[256])
	, m_green(new float[256])
	, m_blue(new float[256])
	, m_alpha(new float[256])
	, m_tickCounter(0)
{
}

TextureWaterFX::~TextureWaterFX() {
	delete[] m_red;
	m_red = NULL;
	delete[] m_green;
	m_green = NULL;
	delete[] m_blue;
	m_blue = NULL;
	delete[] m_alpha;
	m_alpha = NULL;
}

void TextureWaterFX::onTick() {
		++m_tickCounter;

		int var1;
		int var2;
		float var3;
		int var4;
		int var5;
		int var6;
		for(var1 = 0; var1 < 16; ++var1) {
			for(var2 = 0; var2 < 16; ++var2) {
				var3 = 0.0F;

				for(var4 = var1 - 1; var4 <= var1 + 1; ++var4) {
					var5 = var4 & 15;
					var6 = var2 & 15;
					var3 += m_red[var5 + (var6 << 4)];
				}

				m_green[var1 + (var2 << 4)] = var3 / 3.3F + m_blue[var1 + (var2 << 4)] * 0.8F;
			}
		}

		for(var1 = 0; var1 < 16; ++var1) {
			for(var2 = 0; var2 < 16; ++var2) {
				m_blue[var1 + (var2 << 4)] += m_alpha[var1 + (var2 << 4)] * 0.05F;
				if(m_blue[var1 + (var2 << 4)] < 0.0F) {
					m_blue[var1 + (var2 << 4)] = 0.0F;
				}

				m_alpha[var1 + (var2 << 4)] -= 0.1F;
				if(((float)::rand() / (float)RAND_MAX) < 0.05f) {
					m_alpha[var1 + (var2 << 4)] = 0.5F;
				}
			}
		}

		float[] var8 = m_green;
		m_green = m_red;
		m_red = var8;

		for(var2 = 0; var2 < 256; ++var2) {
			var3 = m_red[var2];
			if(var3 > 1.0F) {
				var3 = 1.0F;
			}

			if(var3 < 0.0F) {
				var3 = 0.0F;
			}

			float var9 = var3 * var3;
			var5 = (int)(32.0F + var9 * 32.0F);
			var6 = (int)(50.0F + var9 * 64.0F);
			var1 = 255;
			int var10 = (int)(146.0F + var9 * 50.0F);
			if(anaglyphEnabled) {
				var1 = (var5 * 30 + var6 * 59 + 2805) / 100;
				var4 = (var5 * 30 + var6 * 70) / 100;
				int var7 = (var5 * 30 + 17850) / 100;
				var5 = var1;
				var6 = var4;
				var1 = var7;
			}

			imageData[var2 << 2] = (unsigned char)var5;
			imageData[(var2 << 2) + 1] = (unsigned char)var6;
			imageData[(var2 << 2) + 2] = (unsigned char)var1;
			imageData[(var2 << 2) + 3] = (unsigned char)var10;
		}

}
