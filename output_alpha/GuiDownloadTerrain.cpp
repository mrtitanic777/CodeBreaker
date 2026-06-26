#include "stdafx.h"
// Auto-generated from GuiDownloadTerrain.java
// Target: Xbox 360 / C++03 / DirectX 9
//
// PORT: SINGLEPLAYER 360 port. Keep-alive packet sending is stubbed; this
// screen is only shown while a server streams chunks, which SP never does.

#include "GuiDownloadTerrain.h"
#include "Common.h"
#include "GuiButton.h"
#include "NetClientHandler.h"
#include "GuiScreen.h"

GuiDownloadTerrain::GuiDownloadTerrain()
	: m_netHandler(NULL)
	, m_updateCounter(0)
{
}

GuiDownloadTerrain::GuiDownloadTerrain(NetClientHandler* var1)
	: m_netHandler(NULL)
	, m_updateCounter(0)
{
		m_netHandler = var1;
}

GuiDownloadTerrain::~GuiDownloadTerrain() {
	m_netHandler = NULL;
}

void GuiDownloadTerrain::keyTyped(char var1, int var2) {
	// TODO: implement
}

void GuiDownloadTerrain::initGui() {
  controlList.clear();
}

void GuiDownloadTerrain::updateScreen() {
		++m_updateCounter;
		if(m_updateCounter % 20 == 0) {
			// PORT: 360 MP API TODO -- keep-alive packet (no-op).
		}

		if(m_netHandler != NULL) {
			m_netHandler->processReadPackets();
		}

}

void GuiDownloadTerrain::actionPerformed(GuiButton* var1) {
	// TODO: implement
}

void GuiDownloadTerrain::drawScreen(int var1, int var2, float var3) {
		drawBackground(0);
		drawCenteredString(fontRenderer, "Downloading terrain", width / 2, height / 2 - 50, 16777215);
		GuiScreen::drawScreen(var1, var2, var3);
}
