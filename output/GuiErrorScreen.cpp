#include "stdafx.h"
// Auto-generated from GuiErrorScreen.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiErrorScreen.h"
#include "Common.h"
#include <string>
#include "GuiScreen.h"

GuiErrorScreen::GuiErrorScreen()
{
}

GuiErrorScreen::GuiErrorScreen(const std::string& var1, const std::string& var2)
{
		m_title = var1;
		m_text = var2;
}

GuiErrorScreen::~GuiErrorScreen() {
	// No dynamic resources
}

void GuiErrorScreen::initGui() {
	// TODO: implement
}

void GuiErrorScreen::drawScreen(int var1, int var2, float var3) {
		drawGradientRect(0, 0, width, height, -12574688, -11530224);
		drawCenteredString(fontRenderer, m_title, width / 2, 90, 16777215);
		drawCenteredString(fontRenderer, m_text, width / 2, 110, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}

void GuiErrorScreen::keyTyped(char var1, int var2) {
	// TODO: implement
}
