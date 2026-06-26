#include "stdafx.h"
// Auto-generated from GuiIngameMenu.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiIngameMenu.h"
#include "Common.h"
#include "GuiButton.h"
#include "GuiScreen.h"
#include "World.h"
#include "GuiMainMenu.h"
#include "GuiOptions.h"
#include "Minecraft.h"

GuiIngameMenu::GuiIngameMenu()
	: m_updateCounter2(0)
	, m_updateCounter(0)
{
}

GuiIngameMenu::~GuiIngameMenu() {
	// No dynamic resources
}

void GuiIngameMenu::initGui() {
		m_updateCounter2 = 0;
  controlList.clear();
		controlList.push_back(new GuiButton(1, width / 2 - 100, height / 4 + 48, "Save and quit to title"));
		if(mc->isMultiplayerWorld()) {
			controlList[0]->m_displayString = "Disconnect";
		}

		controlList.push_back(new GuiButton(4, width / 2 - 100, height / 4 + 24, "Back to game"));
		controlList.push_back(new GuiButton(0, width / 2 - 100, height / 4 + 96, "Options..."));
}

void GuiIngameMenu::actionPerformed(GuiButton* var1) {
		if(var1->m_id == 0) {
			mc->displayGuiScreen(new GuiOptions(this, mc->m_gameSettings));
		}

		if(var1->m_id == 1) {
			if(mc->isMultiplayerWorld()) {
				mc->theWorld->sendQuittingDisconnectingPacket();
			}

			mc->func_6261_a(NULL);
			mc->displayGuiScreen(new GuiMainMenu());
		}

		if(var1->m_id == 4) {
			mc->displayGuiScreen(NULL);
			mc->func_6259_e();
		}

}

void GuiIngameMenu::updateScreen() {
		GuiScreen::updateScreen();
		++m_updateCounter;
}

void GuiIngameMenu::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		bool var4 = !mc->theWorld->func_650_a(m_updateCounter2++);
		if(var4 || m_updateCounter < 20) {
			float var5 = ((float)(m_updateCounter % 10) + var3) / 10.0F;
			var5 = sinf(var5 * 3.14159265358979323846f * 2.0F) * 0.2F + 0.8F;
			int var6 = (int)(255.0F * var5);
			drawString(fontRenderer, "Saving level..", 8, height - 16, var6 << 16 | var6 << 8 | var6);
		}

		drawCenteredString(fontRenderer, "Game menu", width / 2, 40, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}
