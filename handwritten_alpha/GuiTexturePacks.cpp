#include "stdafx.h"
// Hand-written PORT override for GuiTexturePacks.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Started from the converter output (ConversionTool\output_alpha\GuiTexturePacks.cpp, ~31 errors)
// and fixed to compile against Alpha's GuiTexturePacks.h, using the already-ported
// handwritten_alpha\GuiMainMenu.cpp for the Alpha Gui idioms (Tesselator& ref, SetTexture +
// Textures::getById, color4f resets, drawString/drawCenteredString from the Gui base).
//
// Notable converter fixes:
//   * Missing includes -> Minecraft / RenderEngine / FontRenderer / TexturePackList / TexturePackBase
//     were forward-declared only ("use of undefined type 'Minecraft'"); pulled the real headers in.
//   * java.io.File + getAbsolutePath() + Sys::openURL() are unportable on the 360 (no filesystem
//     texture-pack folder) -> // PORT: stubbed. The "Open texture pack folder" button is a no-op.
//   * Tesselator has no setColorRGBA_I(rgb, alpha) -> the fade-strip gradients now use color4b()
//     (decode the packed RGB int + pass the alpha byte), and the solid fills use colorI().
//   * Tesselator::getInstance() returns a reference -> "Tesselator& var16/var5".
//   * On a single-player 360 port there are NO external texture packs: availableTexturePacks()
//     returns an empty list (TexturePackList only ever holds the built-in default), so the per-pack
//     row loop simply doesn't run. The screen still renders its frame + title and the Done button
//     returns to the parent screen.
//   * Java mouse drag/scroll (Mouse.isButtonDown) was already // PORT-BLOCKED in the converter
//     output -> left out (XInput-driven scrolling is N/A with an empty list).

#include "GuiTexturePacks.h"
#include "Common.h"
#include "GuiButton.h"
#include "GuiScreen.h"
#include "GuiSmallButton.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "TexturePackBase.h"
#include "TexturePackList.h"
#include "FontRenderer.h"
#include "Minecraft.h"
#include "RenderEngine.h"
#include <string>
#include <vector>

GuiTexturePacks::GuiTexturePacks()
	: m_field_6461_a(NULL)
	, m_field_6460_h(0)
	, m_field_6459_i(32)
	, m_field_6458_j(height - 55 + 4)
	, m_field_6457_l(0)
	, m_field_6456_m(width)
	, m_field_6455_n(-2)
	, m_field_6454_o(-1)
	, m_field_6453_p("")
{
}

GuiTexturePacks::GuiTexturePacks(GuiScreen* var1)
	: m_field_6461_a(NULL)
	, m_field_6460_h(0)
	, m_field_6459_i(32)
	, m_field_6458_j(height - 55 + 4)
	, m_field_6457_l(0)
	, m_field_6456_m(width)
	, m_field_6455_n(-2)
	, m_field_6454_o(-1)
	, m_field_6453_p("")
{
	m_field_6461_a = var1;
}

GuiTexturePacks::~GuiTexturePacks() {
	m_field_6461_a = NULL;
}

void GuiTexturePacks::initGui() {
	controlList.push_back(new GuiSmallButton(5, width / 2 - 154, height - 48, "Open texture pack folder"));
	controlList.push_back(new GuiSmallButton(6, width / 2 + 4, height - 48, "Done"));
	mc->m_texturePackList->func_6532_a();
	// PORT: Java did `new File(mc.minecraftDir, "texturepacks").getAbsolutePath()`. No filesystem
	// texture-pack folder exists on the 360 -> leave the displayed path empty.
	m_field_6453_p = "";
	m_field_6459_i = 32;
	m_field_6458_j = height - 58 + 4;
	m_field_6457_l = 0;
	m_field_6456_m = width;
}

void GuiTexturePacks::actionPerformed(GuiButton* var1) {
	if(var1->m_enabled) {
		if(var1->m_id == 5) {
			// PORT: Sys.openURL("file://" + folder) -> no OS file browser / texturepacks folder on 360.
		}

		if(var1->m_id == 6) {
			// PORT: Java reloaded textures here; the built-in pack is always active on the 360.
			mc->displayGuiScreen(m_field_6461_a);
		}
	}
}

void GuiTexturePacks::mouseClicked(int var1, int var2, int var3) {
	GuiScreen::mouseClicked(var1, var2, var3);
}

void GuiTexturePacks::mouseMovedOrUp(int var1, int var2, int var3) {
	GuiScreen::mouseMovedOrUp(var1, var2, var3);
}

void GuiTexturePacks::drawScreen(int var1, int var2, float var3) {
	drawDefaultBackground();
	if(m_field_6454_o <= 0) {
		mc->m_texturePackList->func_6532_a();
		m_field_6454_o += 20;
	}

	std::vector<void*> var4 = mc->m_texturePackList->availableTexturePacks();
	int var5;
	// PORT: Java handled left-mouse drag/scroll of the pack list here. With no external packs the
	// list is empty, so there is nothing to drag-select; XInput scrolling is N/A. // PORT-BLOCKED.

	var5 = (int)var4.size() * 36 - (m_field_6458_j - m_field_6459_i - 4);
	if(var5 < 0) {
		var5 /= 2;
	}

	if(m_field_6460_h < 0) {
		m_field_6460_h = 0;
	}

	if(m_field_6460_h > var5) {
		m_field_6460_h = var5;
	}

	// Lighting is shader-based on Xbox 360 (disabled)
	Shaders::setFogEnabled(false);
	Tesselator& var16 = Tesselator::getInstance();
	g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/gui/background.png")));
	Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
	float var17 = 32.0F;
	var16.init();
	var16.colorI(2105376);
	var16.vertexUV((float)m_field_6457_l, (float)m_field_6458_j, 0.0f, ((float)m_field_6457_l / var17), ((float)(m_field_6458_j + m_field_6460_h) / var17));
	var16.vertexUV((float)m_field_6456_m, (float)m_field_6458_j, 0.0f, ((float)m_field_6456_m / var17), ((float)(m_field_6458_j + m_field_6460_h) / var17));
	var16.vertexUV((float)m_field_6456_m, (float)m_field_6459_i, 0.0f, ((float)m_field_6456_m / var17), ((float)(m_field_6459_i + m_field_6460_h) / var17));
	var16.vertexUV((float)m_field_6457_l, (float)m_field_6459_i, 0.0f, ((float)m_field_6457_l / var17), ((float)(m_field_6459_i + m_field_6460_h) / var17));
	var16.flush();

	for(size_t var8 = 0; var8 < var4.size(); ++var8) {
		TexturePackBase* var9 = (TexturePackBase*)var4[var8];
		int var10 = width / 2 - 92 - 16;
		int var11 = 36 + (int)var8 * 36 - m_field_6460_h;
		int var12 = 32;
		int var13 = 32;
		if(var9 == mc->m_texturePackList->m_selectedTexturePack) {
			int var14 = width / 2 - 110;
			int var15 = width / 2 + 110;
			Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			g_pd3dDevice->SetTexture(0, NULL);
			var16.init();
			var16.colorI(8421504);
			var16.vertexUV((float)var14, (float)(var11 + var12 + 2), 0.0f, 0.0f, 1.0f);
			var16.vertexUV((float)var15, (float)(var11 + var12 + 2), 0.0f, 1.0f, 1.0f);
			var16.vertexUV((float)var15, (float)(var11 - 2), 0.0f, 1.0f, 0.0f);
			var16.vertexUV((float)var14, (float)(var11 - 2), 0.0f, 0.0f, 0.0f);
			var16.colorI(0);
			var16.vertexUV((float)(var14 + 1), (float)(var11 + var12 + 1), 0.0f, 0.0f, 1.0f);
			var16.vertexUV((float)(var15 - 1), (float)(var11 + var12 + 1), 0.0f, 1.0f, 1.0f);
			var16.vertexUV((float)(var15 - 1), (float)(var11 - 1), 0.0f, 1.0f, 0.0f);
			var16.vertexUV((float)(var14 + 1), (float)(var11 - 1), 0.0f, 0.0f, 0.0f);
			var16.flush();
			g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
		}

		var9->func_6483_c(mc);
		Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		var16.init();
		var16.colorI(16777215);
		var16.vertexUV((float)var10, (float)(var11 + var12), 0.0f, 0.0f, 1.0f);
		var16.vertexUV((float)(var10 + var13), (float)(var11 + var12), 0.0f, 1.0f, 1.0f);
		var16.vertexUV((float)(var10 + var13), (float)var11, 0.0f, 1.0f, 0.0f);
		var16.vertexUV((float)var10, (float)var11, 0.0f, 0.0f, 0.0f);
		var16.flush();
		drawString(fontRenderer, var9->m_texturePackFileName, var10 + var13 + 2, var11 + 1, 16777215);
		drawString(fontRenderer, var9->m_firstDescriptionLine, var10 + var13 + 2, var11 + 12, 8421504);
		drawString(fontRenderer, var9->m_secondDescriptionLine, var10 + var13 + 2, var11 + 12 + 10, 8421504);
	}

	int var18 = 4;
	func_6452_a(0, m_field_6459_i, 255, 255);
	func_6452_a(m_field_6458_j, height, 255, 255);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	// Shade model is shader-based on Xbox 360
	g_pd3dDevice->SetTexture(0, NULL);
	var16.init();
	// PORT: Tesselator has no setColorRGBA_I(rgb, alpha) -> color4b(r,g,b,a). rgb 0 = black.
	var16.color4b(0, 0, 0, 0);
	var16.vertexUV((float)m_field_6457_l, (float)(m_field_6459_i + var18), 0.0f, 0.0f, 1.0f);
	var16.vertexUV((float)m_field_6456_m, (float)(m_field_6459_i + var18), 0.0f, 1.0f, 1.0f);
	var16.color4b(0, 0, 0, 255);
	var16.vertexUV((float)m_field_6456_m, (float)m_field_6459_i, 0.0f, 1.0f, 0.0f);
	var16.vertexUV((float)m_field_6457_l, (float)m_field_6459_i, 0.0f, 0.0f, 0.0f);
	var16.flush();
	var16.init();
	var16.color4b(0, 0, 0, 255);
	var16.vertexUV((float)m_field_6457_l, (float)m_field_6458_j, 0.0f, 0.0f, 1.0f);
	var16.vertexUV((float)m_field_6456_m, (float)m_field_6458_j, 0.0f, 1.0f, 1.0f);
	var16.color4b(0, 0, 0, 0);
	var16.vertexUV((float)m_field_6456_m, (float)(m_field_6458_j - var18), 0.0f, 1.0f, 0.0f);
	var16.vertexUV((float)m_field_6457_l, (float)(m_field_6458_j - var18), 0.0f, 0.0f, 0.0f);
	var16.flush();
	g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
	// Shade model is shader-based on Xbox 360
	g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	drawCenteredString(fontRenderer, "Select Texture Pack", width / 2, 16, 16777215);
	drawCenteredString(fontRenderer, "(Place texture pack files here)", width / 2 - 77, height - 26, 8421504);
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiTexturePacks::updateScreen() {
	GuiScreen::updateScreen();
	--m_field_6454_o;
}

void GuiTexturePacks::func_6452_a(int var1, int var2, int var3, int var4) {
	Tesselator& var5 = Tesselator::getInstance();
	g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/gui/background.png")));
	Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
	float var6 = 32.0F;
	var5.init();
	// PORT: setColorRGBA_I(4210752, alpha) -> color4b. 4210752 = 0x404040 (gray 64,64,64).
	var5.color4b(64, 64, 64, var4);
	var5.vertexUV(0.0f, (float)var2, 0.0f, 0.0f, ((float)var2 / var6));
	var5.vertexUV((float)width, (float)var2, 0.0f, ((float)width / var6), ((float)var2 / var6));
	var5.color4b(64, 64, 64, var3);
	var5.vertexUV((float)width, (float)var1, 0.0f, ((float)width / var6), ((float)var1 / var6));
	var5.vertexUV(0.0f, (float)var1, 0.0f, 0.0f, ((float)var1 / var6));
	var5.flush();
}
