#include "stdafx.h"
// Auto-generated from Gui.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Gui.h"
#include "Common.h"
#include "FontRenderer.h"
#include <string>
#include "Tesselator.h"

Gui::Gui()
	: zLevel(0.0f)
{
}

Gui::~Gui() {
	// No dynamic resources
}

void Gui::drawRect(int var1, int var2, int var3, int var4, int var5) {
		float var6 = (float)(var5 >> 24 & 255) / 255.0F;
		float var7 = (float)(var5 >> 16 & 255) / 255.0F;
		float var8 = (float)(var5 >> 8 & 255) / 255.0F;
		float var9 = (float)(var5 & 255) / 255.0F;
		Tesselator var10 = Tesselator::getInstance();
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetTexture(0, NULL);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  Tesselator::getInstance().color4f(var7, var8, var9, var6);
		var10.init();
		var10.vertex((double)var1, (double)var4, 0.0f);
		var10.vertex((double)var3, (double)var4, 0.0f);
		var10.vertex((double)var3, (double)var2, 0.0f);
		var10.vertex((double)var1, (double)var2, 0.0f);
		var10.flush();
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void Gui::drawGradientRect(int var1, int var2, int var3, int var4, int var5, int var6) {
		float var7 = (float)(var5 >> 24 & 255) / 255.0F;
		float var8 = (float)(var5 >> 16 & 255) / 255.0F;
		float var9 = (float)(var5 >> 8 & 255) / 255.0F;
		float var10 = (float)(var5 & 255) / 255.0F;
		float var11 = (float)(var6 >> 24 & 255) / 255.0F;
		float var12 = (float)(var6 >> 16 & 255) / 255.0F;
		float var13 = (float)(var6 >> 8 & 255) / 255.0F;
		float var14 = (float)(var6 & 255) / 255.0F;
  g_pd3dDevice->SetTexture(0, NULL);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  // Shade model is shader-based on Xbox 360
		Tesselator var15 = Tesselator::getInstance();
		var15.init();
		var15.color4f(var8, var9, var10, var7);
		var15.vertex((double)var3, (double)var2, 0.0f);
		var15.vertex((double)var1, (double)var2, 0.0f);
		var15.color4f(var12, var13, var14, var11);
		var15.vertex((double)var1, (double)var4, 0.0f);
		var15.vertex((double)var3, (double)var4, 0.0f);
		var15.flush();
  // Shade model is shader-based on Xbox 360
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
}

void Gui::drawCenteredString(FontRenderer* var1, const std::string& var2, int var3, int var4, int var5) {
		var1->drawStringWithShadow(var2, var3 - var1->getStringWidth(var2) / 2, var4, var5);
}

void Gui::drawString(FontRenderer* var1, const std::string& var2, int var3, int var4, int var5) {
		var1->drawStringWithShadow(var2, var3, var4, var5);
}

void Gui::drawTexturedModalRect(int var1, int var2, int var3, int var4, int var5, int var6) {
		float var7 = 0.00390625F;
		float var8 = 0.00390625F;
		Tesselator var9 = Tesselator::getInstance();
		var9.init();
		var9.vertexUV((double)(var1 + 0), (double)(var2 + var6), (double)zLevel, (double)((float)(var3 + 0) * var7), (double)((float)(var4 + var6) * var8));
		var9.vertexUV((double)(var1 + var5), (double)(var2 + var6), (double)zLevel, (double)((float)(var3 + var5) * var7), (double)((float)(var4 + var6) * var8));
		var9.vertexUV((double)(var1 + var5), (double)(var2 + 0), (double)zLevel, (double)((float)(var3 + var5) * var7), (double)((float)(var4 + 0) * var8));
		var9.vertexUV((double)(var1 + 0), (double)(var2 + 0), (double)zLevel, (double)((float)(var3 + 0) * var7), (double)((float)(var4 + 0) * var8));
		var9.flush();
}
