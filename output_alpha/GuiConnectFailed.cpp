#include "stdafx.h"
// Auto-generated from GuiConnectFailed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiConnectFailed.h"
#include "Common.h"
#include "GuiButton.h"
#include <string>
#include "GuiScreen.h"
#include "GuiMainMenu.h"
#include "Minecraft.h"

GuiConnectFailed::GuiConnectFailed()
	: m_errorMessage("")
	, m_errorDetail("")
{
}

GuiConnectFailed::GuiConnectFailed(const std::string& var1, const std::string& var2)
	: m_errorMessage("")
	, m_errorDetail("")
{
		m_errorMessage = var1;
		m_errorDetail = var2;
}

GuiConnectFailed::~GuiConnectFailed() {
	// No dynamic resources
}

void GuiConnectFailed::updateScreen() {
	// TODO: implement
}

void GuiConnectFailed::keyTyped(char var1, int var2) {
	// TODO: implement
}

void GuiConnectFailed::initGui() {
  controlList.clear();
		controlList.push_back(new GuiButton(0, width / 2 - 100, height / 4 + 120 + 12, "Back to title screen"));
}

void GuiConnectFailed::actionPerformed(GuiButton* var1) {
		if(var1->m_id == 0) {
			mc->displayGuiScreen(new GuiMainMenu());
		}

}

void GuiConnectFailed::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		drawCenteredString(fontRenderer, m_errorMessage, width / 2, height / 2 - 50, 16777215);
		drawCenteredString(fontRenderer, m_errorDetail, width / 2, height / 2 - 10, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}
