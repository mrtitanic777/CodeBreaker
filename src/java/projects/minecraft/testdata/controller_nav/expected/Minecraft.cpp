#include "stdafx.h"
#include "Minecraft.h"

void Minecraft::displayGuiScreen(GuiScreen* var1)
{
	if (var1 == NULL && this->theWorld == NULL) {
		var1 = new GuiMainMenu();
	}
	if (this->currentScreen != NULL) {
		this->currentScreen->onGuiClosed();
	}
	this->currentScreen = (GuiScreen*)var1;
	Mouse::setGrabbed(var1 == NULL && this->theWorld != NULL);   // CONSOLE: pad grabbed (in-game mode) IFF no screen up AND a world exists -- any menu -> GUI nav mode. Sets g_grabbed DIRECTLY (NOT MouseHelper::grab/ungrab, which use AWT windowComponent/robot -- NULL/broken on console -- and never call setGrabbed anyway, which is why g_grabbed was stuck false). Single source of truth for controller input mode.
	if (var1 != NULL) {
		this->setIngameNotInFocus();
		ScaledResolution var2(this->displayWidth, this->displayHeight);
		var1->setWorldAndResolution(this, var2.getScaledWidth(), var2.getScaledHeight());
	} else {
		this->setIngameFocus();
	}
}

void Minecraft::runTick()
{
	if (this->currentScreen != NULL) {
		this->currentScreen->handleControllerMenuNav();
		this->currentScreen->updateScreen();
	}
	this->theWorld->tick();
}
