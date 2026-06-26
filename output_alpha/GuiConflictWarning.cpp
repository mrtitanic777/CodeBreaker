#include "stdafx.h"
// Auto-generated from GuiConflictWarning.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiConflictWarning.h"
#include "Common.h"
#include "GuiButton.h"
#include "GuiScreen.h"
#include "GuiMainMenu.h"
#include "Minecraft.h"

GuiConflictWarning::GuiConflictWarning()
	: m_updateCounter(0)
{
}

GuiConflictWarning::~GuiConflictWarning() {
	// No dynamic resources
}

void GuiConflictWarning::updateScreen() {
		++m_updateCounter;
}

void GuiConflictWarning::initGui() {
  controlList.clear();
		controlList.push_back(new GuiButton(0, width / 2 - 100, height / 4 + 120 + 12, "Back to title screen"));
}

void GuiConflictWarning::actionPerformed(GuiButton* var1) {
		if(var1->m_enabled) {
			if(var1->m_id == 0) {
				mc->displayGuiScreen(new GuiMainMenu());
			}

		}
}

void GuiConflictWarning::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		drawCenteredString(fontRenderer, "Level save conflict", width / 2, height / 4 - 60 + 20, 16777215);
		drawString(fontRenderer, "Minecraft detected m_a conflict in the level save data.", width / 2 - 140, height / 4 - 60 + 60 + 0, 10526880);
		drawString(fontRenderer, "This could be caused by two copies of the game", width / 2 - 140, height / 4 - 60 + 60 + 18, 10526880);
		drawString(fontRenderer, "accessing the same level.", width / 2 - 140, height / 4 - 60 + 60 + 27, 10526880);
		drawString(fontRenderer, "To prevent level corruption, the current game has quit.", width / 2 - 140, height / 4 - 60 + 60 + 45, 10526880);
		GuiScreen::drawScreen(var1, var2, var3);
}
