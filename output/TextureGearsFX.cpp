#include "stdafx.h"
// Auto-generated from TextureGearsFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureGearsFX.h"
#include "Common.h"
#include "TextureFX.h"

TextureGearsFX::TextureGearsFX()
	: m_gearRotation(0)
	, m_gearColor(new int[1024])
	, m_gearMiddleColor(new int[1024])
	, m_gearRotationDir(0)
{
}

TextureGearsFX::TextureGearsFX(int var1)
	: TextureFX(Block.cog.blockIndexInTexture + var1)
	, m_gearRotation(0)
	, m_gearColor(new int[1024])
	, m_gearMiddleColor(new int[1024])
	, m_gearRotationDir(0)
{
		m_gearRotationDir = (var1 << 1) - 1;
		m_gearRotation = 2;

  { // was try
   // PORT: BufferedImage -> D3D9 texture loading  // was: BufferedImage var3 = ImageIO.read(TextureGearsFX.class.getResource("/misc/gear.png"));
			var3.getRGB(0, 0, 32, 32, m_gearColor, 0, 32);
   // PORT: ImageIO -> D3D9 texture loading  // was: var3 = ImageIO.read(TextureGearsFX.class.getResource("/misc/gearmiddle.png"));
			var3.getRGB(0, 0, 16, 16, m_gearMiddleColor, 0, 16);
		} { DbgPrintF("CATCH IOException\n"); // was catch(IOException var2)
   // Removed Java stream/exception call: var2.printStackTrace();
		}
}

TextureGearsFX::~TextureGearsFX() {
	delete[] m_gearColor;
	m_gearColor = NULL;
	delete[] m_gearMiddleColor;
	m_gearMiddleColor = NULL;
}

void TextureGearsFX::onTick() {
		m_gearRotation = m_gearRotation + m_gearRotationDir & 63;
		float var1 = sinf((float)m_gearRotation / 64.0F * 3.14159265358979323846f * 2.0F);
		float var2 = cosf((float)m_gearRotation / 64.0F * 3.14159265358979323846f * 2.0F);

		for(int var3 = 0; var3 < 16; ++var3) {
			for(int var4 = 0; var4 < 16; ++var4) {
				float var5 = ((float)var3 / 15.0F - 0.5F) * 31.0F;
				float var6 = ((float)var4 / 15.0F - 0.5F) * 31.0F;
				float var7 = var2 * var5 - var1 * var6;
				var5 = var2 * var6 + var1 * var5;
				int var11 = (int)(var7 + 16.0F);
				int var10 = (int)(var5 + 16.0F);
				int var12 = 0;
				if(var11 >= 0 && var10 >= 0 && var11 < 32 && var10 < 32) {
					var12 = m_gearColor[var11 + (var10 << 5)];
					var10 = m_gearMiddleColor[var3 + (var4 << 4)];
					if(var10 >>> 24 > 128) {
						var12 = var10;
					}
				}

				var10 = var12 >> 16 & 255;
				var11 = var12 >> 8 & 255;
				int var8 = var12 & 255;
				var12 = var12 >>> 24 > 128 ? 255 : 0;
				int var9 = var3 + (var4 << 4);
				imageData[var9 << 2] = (unsigned char)var10;
				imageData[(var9 << 2) + 1] = (unsigned char)var11;
				imageData[(var9 << 2) + 2] = (unsigned char)var8;
				imageData[(var9 << 2) + 3] = (unsigned char)var12;
			}
		}

}
