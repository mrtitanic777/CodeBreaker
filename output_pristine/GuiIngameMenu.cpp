#include "stdafx.h"
// Auto-generated from GuiIngameMenu.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiIngameMenu.h"
#include "Common.h"
#include "GuiButton.h"
#include "GuiScreen.h"

GuiIngameMenu::GuiIngameMenu()
{
}

GuiIngameMenu::~GuiIngameMenu() {
	// No dynamic resources
}

void GuiIngameMenu::initGui() {
  memset(controlList, 0, sizeof(controlList));
		controlList.push_back(new GuiButton(0, width / 2 - 100, height / 4, "Options..."));
		controlList.push_back(new GuiButton(1, width / 2 - 100, height / 4 + 24, "Generate new level..."));
		controlList.push_back(new GuiButton(2, width / 2 - 100, height / 4 + 48, "Save level.."));
		controlList.push_back(new GuiButton(3, width / 2 - 100, height / 4 + 72, "Load level.."));
		controlList.push_back(new GuiButton(4, width / 2 - 100, height / 4 + 120, "Back to game"));
		if(mc->m_session == NULL) {
			controlList[2]->m_enabled = false;
			controlList[3]->m_enabled = false;
		}

}

void GuiIngameMenu::actionPerformed(GuiButton* var1) {
		if(var1->m_id == 0) {
			mc->displayGuiScreen(new GuiOptions(*this, mc->m_options));
		}

		if(var1->m_id == 1) {
			mc->displayGuiScreen(new GuiNewLevel(this));
		}

		if(mc->m_session != NULL) {
			if(var1->m_id == 2) {
				mc->displayGuiScreen(new GuiSaveLevel(this));
			}

			if(var1->m_id == 3) {
				mc->displayGuiScreen(new GuiLoadLevel(this));
			}
		}

		if(var1->m_id == 4) {
			mc->displayGuiScreen((GuiScreen)NULL);
			mc->setIngameFocus();
		}

}

void GuiIngameMenu::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		drawCenteredString(fontRenderer, "Game menu", width / 2, 40, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}
