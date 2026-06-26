#include "stdafx.h"
// Auto-generated from GuiControls.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiControls.h"
#include "Common.h"
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiScreen.h"

GuiControls::GuiControls()
	: m_parentScreen(NULL)
	, m_screenTitle("Controls")
	, m_options(NULL)
	, m_buttonId(-1)
{
}

GuiControls::GuiControls(GuiScreen* var1, GameSettings* var2)
	: m_parentScreen(NULL)
	, m_screenTitle("Controls")
	, m_options(NULL)
	, m_buttonId(-1)
{
		m_parentScreen = var1;
		m_options = var2;
}

GuiControls::~GuiControls() {
	m_parentScreen = NULL;
	m_options = NULL;
}

void GuiControls::initGui() {
		for(int var1 = 0; var1 < m_options->m_keyBindings /* PORT: array length - use tracked size */; ++var1) {
			controlList.push_back(new GuiSmallButton(var1, width / 2 - 155 + var1 % 2 * 160, height / 6 + 24 * (var1 >> 1), m_options->setKeyBindingString(var1)));
		}

		controlList.push_back(new GuiButton(200, width / 2 - 100, height / 6 + 168, "Done"));
}

void GuiControls::actionPerformed(GuiButton* var1) {
		for(int var2 = 0; var2 < m_options->m_keyBindings /* PORT: array length - use tracked size */; ++var2) {
			controlList[var2]->m_displayString = m_options->setKeyBindingString(var2);
		}

		if(var1->m_id == 200) {
			mc->displayGuiScreen(m_parentScreen);
		} else {
			m_buttonId = var1->m_id;
			var1->m_displayString = "> " + m_options->setKeyBindingString(var1->m_id) + " <";
		}
}

void GuiControls::keyTyped(char var1, int var2) {
		if(m_buttonId >= 0) {
			m_options->setKeyBinding(m_buttonId, var2);
			controlList[m_buttonId]->m_displayString = m_options->setKeyBindingString(m_buttonId);
			m_buttonId = -1;
		} else {
			GuiScreen::keyTyped(var1, var2);
		}
}

void GuiControls::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		drawCenteredString(fontRenderer, m_screenTitle, width / 2, 20, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}
