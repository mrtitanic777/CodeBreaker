#include "stdafx.h"
// Auto-generated from FontRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "FontRenderer.h"
#include "Common.h"
#include "GameSettings.h"
#include "RenderEngine.h"
#include <string>
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"

FontRenderer::FontRenderer()
	: m_charWidth(new int[256])
	, m_fontTextureName(0)
	, m_fontDisplayLists(0)
	, m_buffer(new int[1024])
{
}

FontRenderer::FontRenderer(GameSettings* var1, const std::string& var2, RenderEngine* var3)
	: m_charWidth(new int[256])
	, m_fontTextureName(0)
	, m_fontDisplayLists(0)
	, m_buffer(new int[1024])
{
  // PORT: BufferedImage -> D3D9 texture loading  // was: BufferedImage var4;
  { // was try
   // PORT: ImageIO -> D3D9 texture loading  // was: var4 = ImageIO.read(RenderEngine.class.getResourceAsStream(var2));
		} { DbgPrintF("CATCH IOException\n"); // was catch(IOException var15)
			DbgPrintF("ERROR [RuntimeException]: %s\n", var15);
		}

		int var5 = var4.getWidth();
		int var6 = var4.getHeight();
		int* var7 = new int[var5 * var6];
		var4.getRGB(0, 0, var5, var6, var7, 0, var5);

		int var8;
		int var9;
		int var11;
		int var13;
		int var14;
		for(int var17 = 0; var17 < 128; ++var17) {
			var6 = var17 % 16;
			var8 = var17 / 16;
			var9 = 0;

			for(bool var10 = false; var9 < 8 && !var10; ++var9) {
				var11 = (var6 << 3) + var9;
				var10 = true;

				for(int var12 = 0; var12 < 8 && var10; ++var12) {
					var13 = ((var8 << 3) + var12) * var5;
					var14 = var7[var11 + var13] & 255;
					if(var14 > 128) {
						var10 = false;
					}
				}
			}

			if(var17 == 32) {
				var9 = 4;
			}

			m_charWidth[var17] = var9;
		}

		m_fontTextureName = var3->getTexture(var2);
  // PORT: allocate 288 vertex buffer slots
		Tesselator var18 = Tesselator::getInstance();

		for(var6 = 0; var6 < 256; ++var6) {
   // BEGIN display list -> vertex buffer recording
			var18.init();
			var8 = var6 % 16 << 3;
			var9 = var6 / 16 << 3;
			var18.vertexUV(0.0F, 7.99F, 0.0F, (float)var8 / 128.0F, ((float)var9 + 7.99F) / 128.0F);
			var18.vertexUV(7.99F, 7.99F, 0.0F, ((float)var8 + 7.99F) / 128.0F, ((float)var9 + 7.99F) / 128.0F);
			var18.vertexUV(7.99F, 0.0F, 0.0F, ((float)var8 + 7.99F) / 128.0F, (float)var9 / 128.0F);
			var18.vertexUV(0.0F, 0.0F, 0.0F, (float)var8 / 128.0F, (float)var9 / 128.0F);
			var18.flush();
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)this->m_charWidth[var6], 0.0F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   // END display list -> vertex buffer recording
		}

		for(var6 = 0; var6 < 32; ++var6) {
			var8 = (var6 & 8) << 3;
			var9 = (var6 & 1) * 191 + var8;
			int var19 = ((var6 & 2) >> 1) * 191 + var8;
			var11 = ((var6 & 4) >> 2) * 191 + var8;
			bool var20 = var6 >= 16;
			if(var1->m_anaglyph) {
				var13 = (var11 * 30 + var19 * 59 + var9 * 11) / 100;
				var14 = (var11 * 30 + var19 * 70) / 100;
				int var16 = (var11 * 30 + var9 * 70) / 100;
				var11 = var13;
				var19 = var14;
				var9 = var16;
			}

			var6 += 2;
			if(var20) {
				var11 /= 4;
				var19 /= 4;
				var9 /= 4;
			}

   Tesselator::getInstance().color4f((float);
		}

}

FontRenderer::~FontRenderer() {
	delete[] m_charWidth;
	m_charWidth = NULL;
}

void FontRenderer::drawStringWithShadow(const std::string& var1, int var2, int var3, int var4) {
		renderString(var1, var2 + 1, var3 + 1, var4, true);
		drawString(var1, var2, var3, var4);
}

void FontRenderer::drawString(const std::string& var1, int var2, int var3, int var4) {
		renderString(var1, var2, var3, var4, false);
}

void FontRenderer::renderString(const std::string& var1, int var2, int var3, int var4, bool var5) {
		if(var1 != NULL) {
			char[] var8 = var1.c_str();
			if(var5) {
				var4 = (var4 & 16579836) >> 2;
			}

   g_pd3dDevice->SetTexture(0, Textures::getById(this->m_fontTextureName));
			float var6 = (float)(var4 >> 16 & 255) / 255.0F;
			float var7 = (float)(var4 >> 8 & 255) / 255.0F;
			float var9 = (float)(var4 & 255) / 255.0F;
   Tesselator::getInstance().color4f(var6, var7, var9, 1.0F);
   memset(m_buffer, 0, sizeof(m_buffer));
   D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var3, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }

			for(int var10 = 0; var10 < var8_count /* PORT: set this from allocation */; ++var10) {
				for(; var8[var10] == 38 && var8 /* PORT: array length - use tracked size */ > var10 + 1; var10 += 2) {
					int var11 = "0123456789abcdef".indexOf(var8[var10 + 1]);
					if(var11 < 0 || var11 > 15) {
						var11 = 15;
					}

     m_buffer[0] = m_fontDisplayLists + 256 + var11 + (var5 ? 16 : 0;
     // NIO remaining() � no-op for C array
      // NIO flip() � no-op for C array
      // PORT: batch draw cached vertex buffers
      memset(m_buffer, 0, sizeof(m_buffer));
					}
				}

    m_buffer[0] = m_fontDisplayLists + var8[var10];
    // NIO remaining() � no-op for C array
     // NIO flip() � no-op for C array
     // PORT: batch draw cached vertex buffers
     memset(m_buffer, 0, sizeof(m_buffer));
				}
			}

   // NIO flip() � no-op for C array
   // PORT: batch draw cached vertex buffers
   Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		}
}

int FontRenderer::getStringWidth(const std::string& var1) {
		if(var1 == NULL) {
			return 0;
		} else {
			char[] var4 = var1.c_str();
			int var2 = 0;

			for(int var3 = 0; var3 < var4_count /* PORT: set this from allocation */; ++var3) {
				if(var4[var3] == 38) {
					++var3;
				} else {
					var2 += m_charWidth[var4[var3]];
				}
			}

			return var2;
		}
}
