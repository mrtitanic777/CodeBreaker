#include "stdafx.h"
#include "GuiButton.h"
#include "Input.h"
#include "GuiScreen.h"
#include "Minecraft.h"

GuiScreen::GuiScreen()
	: width(0)
	, height(0)
	, allowUserInput(false)
	, navSelected(-1)
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

// CONSOLE (controller menu nav) -- CodeBreaker-injected (controller_nav.py). Feeds this screen's button
// rectangles to the edition-agnostic Input::menuNavSelect core (Input.reference) and snaps the virtual
// cursor onto the D-pad selection, so the game's OWN hover-highlight + A-as-click land on it. Migrated
// from the per-project hand-port overlay so every edition gets working menu nav from one place.
void GuiScreen::handleControllerMenuNav() {
	int n = (int)this->controlList.size();
	if (n <= 0) { this->navSelected = -1; return; }
	if (n > 128) n = 128;
	int bx[128], by[128], bw[128], bh[128]; unsigned char en[128];
	for (int i = 0; i < n; ++i) {
		GuiButton* b = (GuiButton*)this->controlList[i];
		bx[i] = b ? b->xPosition : 0;  by[i] = b ? b->yPosition : 0;
		bw[i] = b ? b->width : 0;      bh[i] = b ? b->height : 0;
		en[i] = (b && b->enabled) ? 1 : 0;
	}
	this->navSelected = Input::menuNavSelect(bx, by, bw, bh, en, n, this->navSelected);
	if (this->navSelected >= 0 && this->navSelected < n && this->mc != NULL) {
		GuiButton* b = (GuiButton*)this->controlList[this->navSelected];
		if (b != NULL) Input::menuSnapCursor(b->xPosition + b->width / 2, b->yPosition + b->height / 2,
			this->width, this->height, this->mc->displayWidth, this->mc->displayHeight);
	}
}
