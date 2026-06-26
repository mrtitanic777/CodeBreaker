#include "stdafx.h"
// Auto-generated from GuiOptions.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiOptions.h"
#include "Common.h"
#include "GameSettings.h"
#include "GuiButton.h"
#include "GuiScreen.h"

GuiOptions::GuiOptions()
	: m_parentScreen(NULL)
	, m_screenTitle("Options")
	, m_options(NULL)
{
}

GuiOptions::GuiOptions(GuiScreen* var1, GameSettings* var2)
	: m_parentScreen(NULL)
	, m_screenTitle("Options")
	, m_options(NULL)
{
		m_parentScreen = var1;
		m_options = var2;
}

GuiOptions::~GuiOptions() {
	m_parentScreen = NULL;
	m_options = NULL;
}

void GuiOptions::initGui() {
		for(int var1 = 0; var1 < m_options->m_numberOfOptions; ++var1) {
			controlList.push_back(new GuiSmallButton(var1, width / 2 - 155 + var1 % 2 * 160, height / 6 + 24 * (var1 >> 1), m_options->setOptionString(var1)));
		}

		controlList.push_back(new GuiButton(100, width / 2 - 100, height / 6 + 120 + 12, "Controls..."));
		controlList.push_back(new GuiButton(200, width / 2 - 100, height / 6 + 168, "Done"));
}

void GuiOptions::actionPerformed(GuiButton* var1) {
		if(var1->m_enabled) {
			if(var1->m_id < 100) {
				m_options->setOptionValue(var1->m_id, 1);
				var1->m_displayString = m_options->setOptionString(var1->m_id);
			}

			if(var1->m_id == 100) {
				mc->displayGuiScreen(new GuiControls(*this, m_options));
			}

			if(var1->m_id == 200) {
				mc->displayGuiScreen(m_parentScreen);
			}

		}
}

void GuiOptions::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		drawCenteredString(fontRenderer, m_screenTitle, width / 2, 20, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}
