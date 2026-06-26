#include "stdafx.h"
// Auto-generated from GuiMainMenu.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiMainMenu.h"
#include "Common.h"
#include "GuiButton.h"
#include <string>
#include "GuiScreen.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "FontRenderer.h"
#include "GuiLoadLevel.h"
#include "GuiNewLevel.h"
#include "GuiOptions.h"
#include "Minecraft.h"
#include "RenderEngine.h"

GuiMainMenu::GuiMainMenu()
	: m_updateCounter(0.0f)
	, m_splashes("")
	, m_currentSplash("Now with shaders!")
{
}

GuiMainMenu::~GuiMainMenu() {
	m_splashes.clear();
}

void GuiMainMenu::updateScreen() {
		m_updateCounter += 0.01F;
}

void GuiMainMenu::keyTyped(char var1, int var2) {
	// TODO: implement
}

void GuiMainMenu::initGui() {
  controlList.clear();
		controlList.push_back(new GuiButton(1, width / 2 - 100, height / 4 + 48, "Generate new level..."));
		controlList.push_back(new GuiButton(2, width / 2 - 100, height / 4 + 72, "Load level.."));
		controlList.push_back(new GuiButton(3, width / 2 - 100, height / 4 + 96, "Play tutorial level"));
		controlList.push_back(new GuiButton(0, width / 2 - 100, height / 4 + 120 + 12, "Options..."));
		controlList[2]->m_enabled = false;
		if(mc->m_session == NULL) {
			controlList[1]->m_enabled = false;
		}

}

void GuiMainMenu::actionPerformed(GuiButton* var1) {
		if(var1->m_id == 0) {
			mc->displayGuiScreen(new GuiOptions(this, mc->m_options));
		}

		if(var1->m_id == 1) {
			mc->displayGuiScreen(new GuiNewLevel(this));
		}

		if(mc->m_session != NULL && var1->m_id == 2) {
			mc->displayGuiScreen(new GuiLoadLevel(this));
		}

}

void GuiMainMenu::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		Tesselator var4 = Tesselator::getInstance();
  g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/gui/logo.png")));
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		var4.colorI(16777215);
		drawTexturedModalRect((width - 256) / 2, 30, 0, 0, 256, 49);
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(this->width / 2 + 90), 70.0F, 0.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (-20.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		float var15 = 1.8F - abs(sinf((float)(GetTickCount() % 1000L) / 1000.0F * 3.14159265358979323846f * 2.0F) * 0.1F);
		var15 = var15 * 100.0F / (float)(fontRenderer->getStringWidth(m_currentSplash) + 32);
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var15, var15, var15);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		drawCenteredString(fontRenderer, m_currentSplash, 0, -8, 16776960);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		std::string var16 = "Copyright Mojang Specifications. Do not distribute.";
		drawString(fontRenderer, var16, width - fontRenderer->getStringWidth(var16) - 2, height - 10, 16777215);
		long var7 = 64 * 1024 * 1024 /* 64MB */;
		long var9 = 64 * 1024 * 1024 /* 64MB */;
		long var11 = 32 * 1024 * 1024 /* 32MB */;
		long var13 = var7 - var11;
		var16 = "Free memory: " + toStr(var13 * 100L / var7) + "% of " + toStr(var7 / 1024L / 1024L) + "MB";
		drawString(fontRenderer, var16, width - fontRenderer->getStringWidth(var16) - 2, 2, 8421504);
		var16 = "Allocated memory: " + toStr(var9 * 100L / var7) + "% (" + toStr(var9 / 1024L / 1024L) + "MB)";
		drawString(fontRenderer, var16, width - fontRenderer->getStringWidth(var16) - 2, 12, 8421504);
		GuiScreen::drawScreen(var1, var2, var3);
}
