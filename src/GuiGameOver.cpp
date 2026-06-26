#include "stdafx.h"
// Auto-generated from GuiGameOver.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiGameOver.h"
#include "Common.h"
#include "GuiButton.h"
#include <string>
#include "EntityPlayerSP.h"
#include "FontRenderer.h"
#include "GuiScreen.h"
#include "Shaders.h"
#include "GuiOptions.h"
#include "Minecraft.h"

GuiGameOver::GuiGameOver()
{
}

GuiGameOver::~GuiGameOver() {
	// No dynamic resources
}

void GuiGameOver::initGui() {
  memset(controlList, 0, sizeof(controlList));
		controlList.push_back(new GuiButton(1, width / 2 - 100, height / 4 + 72, "Generate new level..."));
		controlList.push_back(new GuiButton(2, width / 2 - 100, height / 4 + 96, "Load level.."));
		if(mc->m_session == NULL) {
			controlList[1]->m_enabled = false;
		}

}

void GuiGameOver::keyTyped(char var1, int var2) {
	// TODO: implement
}

void GuiGameOver::actionPerformed(GuiButton* var1) {
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

void GuiGameOver::drawScreen(int var1, int var2, float var3) {
		drawGradientRect(0, 0, width, height, 1615855616, -1602211792);
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 2.0F, 2.0F, 2.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		drawCenteredString(fontRenderer, "Game over!", width / 2 / 2, 30, 16777215);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		FontRenderer var10000 = fontRenderer;
		std::string var10001 = (new std::string()).append("Score: &e");
		EntityPlayerSP* var4 = mc->thePlayer;
		drawCenteredString(var10000, var10001.append(var4->m_getScore) /* PORT: toString */, width / 2, 100, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}

bool GuiGameOver::doesGuiPauseGame() {
		return false;
}
