#include "stdafx.h"
// Auto-generated from TextureLavaFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureLavaFX.h"
#include "Common.h"
#include "Block.h"
#include "TextureFX.h"

TextureLavaFX::TextureLavaFX()
	: TextureFX(Block::s_lavaMoving->blockIndexInTexture)
	, m_red(new float[256])
	, m_green(new float[256])
	, m_blue(new float[256])
	, m_alpha(new float[256])
{
}

TextureLavaFX::~TextureLavaFX() {
	delete[] m_red;
	m_red = NULL;
	delete[] m_green;
	m_green = NULL;
	delete[] m_blue;
	m_blue = NULL;
	delete[] m_alpha;
	m_alpha = NULL;
}

void TextureLavaFX::onTick() {
		int var1;
		int var2;
		float var3;
		int var5;
		int var6;
		int var7;
		int var8;
		int var9;
		for(var1 = 0; var1 < 16; ++var1) {
			for(var2 = 0; var2 < 16; ++var2) {
				var3 = 0.0F;
				int var4 = (int)(sinf((float)var2 * 3.14159265358979323846f * 2.0F / 16.0F) * 1.2F);
				var5 = (int)(sinf((float)var1 * 3.14159265358979323846f * 2.0F / 16.0F) * 1.2F);

				for(var6 = var1 - 1; var6 <= var1 + 1; ++var6) {
					for(var7 = var2 - 1; var7 <= var2 + 1; ++var7) {
						var8 = var6 + var4 & 15;
						var9 = var7 + var5 & 15;
						var3 += m_red[var8 + (var9 << 4)];
					}
				}

				m_green[var1 + (var2 << 4)] = var3 / 10.0F + (m_blue[(var1 & 15) + ((var2 & 15) << 4)] + m_blue[(var1 + 1 & 15) + ((var2 & 15) << 4)] + m_blue[(var1 + 1 & 15) + ((var2 + 1 & 15) << 4)] + m_blue[(var1 & 15) + ((var2 + 1 & 15) << 4)]) / 4.0F * 0.8F;
				m_blue[var1 + (var2 << 4)] += m_alpha[var1 + (var2 << 4)] * 0.01F;
				if(m_blue[var1 + (var2 << 4)] < 0.0F) {
					m_blue[var1 + (var2 << 4)] = 0.0F;
				}

				m_alpha[var1 + (var2 << 4)] -= 0.06F;
				if(((float)::rand() / (float)RAND_MAX) < 0.005f) {
					m_alpha[var1 + (var2 << 4)] = 1.5F;
				}
			}
		}

		float[] var10 = m_green;
		m_green = m_red;
		m_red = var10;

		for(var2 = 0; var2 < 256; ++var2) {
			var3 = m_red[var2] * 2.0F;
			if(var3 > 1.0F) {
				var3 = 1.0F;
			}

			if(var3 < 0.0F) {
				var3 = 0.0F;
			}

			var5 = (int)(var3 * 100.0F + 155.0F);
			var6 = (int)(var3 * var3 * 255.0F);
			var7 = (int)(var3 * var3 * var3 * var3 * 128.0F);
			if(anaglyphEnabled) {
				var8 = (var5 * 30 + var6 * 59 + var7 * 11) / 100;
				var9 = (var5 * 30 + var6 * 70) / 100;
				var1 = (var5 * 30 + var7 * 70) / 100;
				var5 = var8;
				var6 = var9;
				var7 = var1;
			}

			imageData[var2 << 2] = (unsigned char)var5;
			imageData[(var2 << 2) + 1] = (unsigned char)var6;
			imageData[(var2 << 2) + 2] = (unsigned char)var7;
			imageData[(var2 << 2) + 3] = -1;
		}

}
