#include "stdafx.h"
// Auto-generated from GuiYesNo.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiYesNo.h"
#include "Common.h"
#include "GuiButton.h"
#include <string>
#include "GuiScreen.h"
#include "GuiSmallButton.h"

GuiYesNo::GuiYesNo()
	: m_parentScreen(NULL)
	, m_message1("")
	, m_message2("")
	, m_worldNumber(0)
{
}

GuiYesNo::GuiYesNo(GuiScreen* var1, const std::string& var2, const std::string& var3, int var4)
	: m_parentScreen(NULL)
	, m_message1("")
	, m_message2("")
	, m_worldNumber(0)
{
		m_parentScreen = var1;
		m_message1 = var2;
		m_message2 = var3;
		m_worldNumber = var4;
}

GuiYesNo::~GuiYesNo() {
	m_parentScreen = NULL;
}

void GuiYesNo::initGui() {
		controlList.push_back(new GuiSmallButton(0, width / 2 - 155 + 0, height / 6 + 96, "Yes"));
		controlList.push_back(new GuiSmallButton(1, width / 2 - 155 + 160, height / 6 + 96, "No"));
}

void GuiYesNo::actionPerformed(GuiButton* var1) {
		m_parentScreen->deleteWorld(var1->m_id == 0, m_worldNumber);
}

void GuiYesNo::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		drawCenteredString(fontRenderer, m_message1, width / 2, 70, 16777215);
		drawCenteredString(fontRenderer, m_message2, width / 2, 90, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}
