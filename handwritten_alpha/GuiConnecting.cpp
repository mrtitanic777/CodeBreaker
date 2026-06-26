#include "stdafx.h"
// Auto-generated from GuiConnecting.java
// Target: Xbox 360 / C++03 / DirectX 9
//
// PORT: SINGLEPLAYER 360 port. The connect screen normally spawns a
// ThreadConnectToServer (not generated) and pumps a NetClientHandler. Both are
// stubbed -- in SP this screen is never shown; it just compiles.

#include "GuiConnecting.h"
#include "Common.h"
#include "GuiButton.h"
#include "Minecraft.h"
#include "NetClientHandler.h"
#include <string>
#include "GuiScreen.h"
#include "World.h"
#include "GuiMainMenu.h"

GuiConnecting::GuiConnecting()
	: m_clientHandler(NULL)
	, m_cancelled(false)
{
}

GuiConnecting::GuiConnecting(Minecraft* var1, const std::string& var2, int var3)
	: m_clientHandler(NULL)
	, m_cancelled(false)
{
		var1->func_6261_a(NULL);
		// PORT: 360 MP API TODO -- (new ThreadConnectToServer(...))->start();
		// ThreadConnectToServer is not generated (skip_files). No-op in SP.
		(void)var2;
		(void)var3;
}

GuiConnecting::~GuiConnecting() {
	m_clientHandler = NULL;
}

void GuiConnecting::updateScreen() {
		if(m_clientHandler != NULL) {
			m_clientHandler->processReadPackets();
		}

}

void GuiConnecting::keyTyped(char var1, int var2) {
	// TODO: implement
}

void GuiConnecting::initGui() {
  controlList.clear();
		controlList.push_back(new GuiButton(0, width / 2 - 100, height / 4 + 120 + 12, "Cancel"));
}

void GuiConnecting::actionPerformed(GuiButton* var1) {
		if(var1->m_id == 0) {
			m_cancelled = true;
			if(m_clientHandler != NULL) {
				m_clientHandler->disconnect();
			}

			mc->displayGuiScreen(new GuiMainMenu());
		}

}

void GuiConnecting::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		if(m_clientHandler == NULL) {
			drawCenteredString(fontRenderer, "Connecting to the server...", width / 2, height / 2 - 50, 16777215);
			drawCenteredString(fontRenderer, "", width / 2, height / 2 - 10, 16777215);
		} else {
			drawCenteredString(fontRenderer, "Logging in...", width / 2, height / 2 - 50, 16777215);
			drawCenteredString(fontRenderer, m_clientHandler->field_1209_a, width / 2, height / 2 - 10, 16777215);
		}

		GuiScreen::drawScreen(var1, var2, var3);
}

NetClientHandler* GuiConnecting::setNetClientHandler(GuiConnecting* var0, NetClientHandler* var1) {
		return var0->m_clientHandler = var1;
}

bool GuiConnecting::isCancelled(GuiConnecting* var0) {
		return var0->m_cancelled;
}

NetClientHandler* GuiConnecting::getNetClientHandler(GuiConnecting* var0) {
		return var0->m_clientHandler;
}
