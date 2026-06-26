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

void Gui::drawRect(int var0, int var1, int var2, int var3, int var4) {
		float var5 = (float)((unsigned int)(var4) >> 24) / 255.0F;
		float var6 = (float)(var4 >> 16 & 255) / 255.0F;
		float var7 = (float)(var4 >> 8 & 255) / 255.0F;
		float var9 = (float)(var4 & 255) / 255.0F;
		Tesselator var8 = Tesselator::getInstance();
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetTexture(0, NULL);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  Tesselator::getInstance().color4f(var6, var7, var9, var5);
		var8.init();
		var8.vertex((float)var0, (float)var3, 0.0F);
		var8.vertex((float)var2, (float)var3, 0.0F);
		var8.vertex((float)var2, (float)var1, 0.0F);
		var8.vertex((float)var0, (float)var1, 0.0F);
		var8.flush();
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void Gui::drawGradientRect(int var0, int var1, int var2, int var3, int var4, int var5) {
		float var6 = (float)((unsigned int)(var4) >> 24) / 255.0F;
		float var7 = (float)(var4 >> 16 & 255) / 255.0F;
		float var8 = (float)(var4 >> 8 & 255) / 255.0F;
		float var13 = (float)(var4 & 255) / 255.0F;
		float var9 = (float)((unsigned int)(var5) >> 24) / 255.0F;
		float var10 = (float)(var5 >> 16 & 255) / 255.0F;
		float var11 = (float)(var5 >> 8 & 255) / 255.0F;
		float var14 = (float)(var5 & 255) / 255.0F;
  g_pd3dDevice->SetTexture(0, NULL);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		Tesselator var12 = Tesselator::getInstance();
		var12.init();
		var12.color4f(var7, var8, var13, var6);
		var12.vertex((float)var2, (float)var1, 0.0F);
		var12.vertex((float)var0, (float)var1, 0.0F);
		var12.color4f(var10, var11, var14, var9);
		var12.vertex((float)var0, (float)var3, 0.0F);
		var12.vertex((float)var2, (float)var3, 0.0F);
		var12.flush();
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
}

void Gui::drawCenteredString(FontRenderer* var0, const std::string& var1, int var2, int var3, int var4) {
		var0->drawStringWithShadow(var1, var2 - var0->getStringWidth(var1) / 2, var3, var4);
}

void Gui::drawString(FontRenderer* var0, const std::string& var1, int var2, int var3, int var4) {
		var0->drawStringWithShadow(var1, var2, var3, var4);
}

void Gui::drawTexturedModalRect(int var1, int var2, int var3, int var4, int var5, int var6) {
		Tesselator var7 = Tesselator::getInstance();
		var7.init();
		var7.vertexUV((float)var1, (float)(var2 + var6), zLevel, (float)var3 * 0.00390625F, (float)(var4 + var6) * 0.00390625F);
		var7.vertexUV((float)(var1 + var5), (float)(var2 + var6), zLevel, (float)(var3 + var5) * 0.00390625F, (float)(var4 + var6) * 0.00390625F);
		var7.vertexUV((float)(var1 + var5), (float)var2, zLevel, (float)(var3 + var5) * 0.00390625F, (float)var4 * 0.00390625F);
		var7.vertexUV((float)var1, (float)var2, zLevel, (float)var3 * 0.00390625F, (float)var4 * 0.00390625F);
		var7.flush();
}
