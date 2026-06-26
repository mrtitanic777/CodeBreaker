#include "stdafx.h"
// Auto-generated from GuiGameOver.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiGameOver.h"
#include "Common.h"
#include "GuiButton.h"
#include "GuiScreen.h"
#include "Shaders.h"
#include "World.h"
#include "EntityPlayerSP.h"
#include "GuiMainMenu.h"
#include "GuiOptions.h"
#include "Minecraft.h"

GuiGameOver::GuiGameOver()
{
}

GuiGameOver::~GuiGameOver() {
	// No dynamic resources
}

void GuiGameOver::initGui() {
  controlList.clear();
		controlList.push_back(new GuiButton(1, width / 2 - 100, height / 4 + 72, "Respawn"));
		controlList.push_back(new GuiButton(2, width / 2 - 100, height / 4 + 96, "Title menu"));
		if(mc->m_field_6320_i == NULL) {
			controlList[1]->m_enabled = false;
		}

}

void GuiGameOver::keyTyped(char var1, int var2) {
	// TODO: implement
}

void GuiGameOver::actionPerformed(GuiButton* var1) {
		if(var1->m_id == 0) {
		}

		if(var1->m_id == 1) {
			mc->thePlayer->func_9367_r();
			mc->displayGuiScreen(NULL);
		}

		if(var1->m_id == 2) {
			mc->func_6261_a(NULL);
			mc->displayGuiScreen(new GuiMainMenu());
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
		drawCenteredString(fontRenderer, "Score: &e" + mc->thePlayer->func_6417_t(), width / 2, 100, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}

bool GuiGameOver::doesGuiPauseGame() {
		return false;
}
