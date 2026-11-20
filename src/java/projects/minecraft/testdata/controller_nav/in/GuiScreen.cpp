#include "stdafx.h"
#include "GuiScreen.h"
#include "Minecraft.h"

GuiScreen::GuiScreen()
	: width(0)
	, height(0)
	, allowUserInput(false)
{
	this->mc = NULL;
}

void GuiScreen::drawScreen(int x, int y, float a)
{
	for (int i = 0; i < (int)this->controlList.size(); ++i) {
		((GuiButton*)this->controlList[i])->drawButton(this->mc, x, y);
	}
}

void GuiScreen::updateScreen()
{
}
