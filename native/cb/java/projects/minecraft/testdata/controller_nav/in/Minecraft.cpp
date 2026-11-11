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
		this->currentScreen->updateScreen();
	}
	this->theWorld->tick();
}
