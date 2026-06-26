#include "stdafx.h"
// Auto-generated from GuiUnused.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiUnused.h"
#include "Common.h"
#include "GuiScreen.h"

GuiUnused::GuiUnused()
	: m_message1("")
	, m_message2("")
{
}

GuiUnused::~GuiUnused() {
	// No dynamic resources
}

void GuiUnused::initGui() {
	// TODO: implement
}

void GuiUnused::drawScreen(int var1, int var2, float var3) {
		drawGradientRect(0, 0, width, height, -12574688, -11530224);
		drawCenteredString(fontRenderer, m_message1, width / 2, 90, 16777215);
		drawCenteredString(fontRenderer, m_message2, width / 2, 110, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}

void GuiUnused::keyTyped(char var1, int var2) {
	// TODO: implement
}
