#include "stdafx.h"
// Auto-generated from TextureFlamesFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFlamesFX.h"
#include "Common.h"
#include "Block.h"
#include "TextureFX.h"

TextureFlamesFX::TextureFlamesFX()
	: m_currentFireFrame(new float[320])
	, m_lastFireFrame(new float[320])
{
}

TextureFlamesFX::TextureFlamesFX(int var1)
	: TextureFX(Block::s_fire->blockIndexInTexture + (var1 << 4))
	, m_currentFireFrame(new float[320])
	, m_lastFireFrame(new float[320])
{
}

TextureFlamesFX::~TextureFlamesFX() {
	delete[] m_currentFireFrame;
	m_currentFireFrame = NULL;
	delete[] m_lastFireFrame;
	m_lastFireFrame = NULL;
}

void TextureFlamesFX::onTick() {
		int var1;
		int var2;
		int var3;
		int var5;
		int var6;
		for(var1 = 0; var1 < 16; ++var1) {
			for(var2 = 0; var2 < 20; ++var2) {
				var3 = 18;
				float var4 = m_currentFireFrame[var1 + ((var2 + 1) % 20 << 4)] * 18.0F;

				for(var5 = var1 - 1; var5 <= var1 + 1; ++var5) {
					for(var6 = var2; var6 <= var2 + 1; ++var6) {
						if(var5 >= 0 && var6 >= 0 && var5 < 16 && var6 < 20) {
							var4 += m_currentFireFrame[var5 + (var6 << 4)];
						}

						++var3;
					}
				}

				m_lastFireFrame[var1 + (var2 << 4)] = var4 / ((float)var3 * 1.06F);
				if(var2 >= 19) {
					m_lastFireFrame[var1 + (var2 << 4)] = (float)(((float)::rand() / (float)RAND_MAX) * ((float)::rand() / (float)RAND_MAX) * ((float)::rand() / (float)RAND_MAX) * 4.0f + ((float)::rand() / (float)RAND_MAX) * (double)0.1F + (double)0.2F);
				}
			}
		}

		float[] var9 = m_lastFireFrame;
		m_lastFireFrame = m_currentFireFrame;
		m_currentFireFrame = var9;

		for(var2 = 0; var2 < 256; ++var2) {
			float var10 = m_currentFireFrame[var2] * 1.8F;
			if(var10 > 1.0F) {
				var10 = 1.0F;
			}

			if(var10 < 0.0F) {
				var10 = 0.0F;
			}

			var5 = (int)(var10 * 155.0F + 100.0F);
			var6 = (int)(var10 * var10 * 255.0F);
			int var7 = (int)(var10 * var10 * var10 * var10 * var10 * var10 * var10 * var10 * var10 * var10 * 255.0F);
			short var8 = 255;
			if(var10 < 0.5F) {
				var8 = 0;
			}

			if(anaglyphEnabled) {
				var1 = (var5 * 30 + var6 * 59 + var7 * 11) / 100;
				var3 = (var5 * 30 + var6 * 70) / 100;
				int var11 = (var5 * 30 + var7 * 70) / 100;
				var5 = var1;
				var6 = var3;
				var7 = var11;
			}

			imageData[var2 << 2] = (unsigned char)var5;
			imageData[(var2 << 2) + 1] = (unsigned char)var6;
			imageData[(var2 << 2) + 2] = (unsigned char)var7;
			imageData[(var2 << 2) + 3] = (unsigned char)var8;
		}

}
