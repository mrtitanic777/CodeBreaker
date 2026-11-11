#ifndef GUISCREEN_H
#define GUISCREEN_H

#include "Gui.h"
#include "JList.h"

class Minecraft;
class GuiButton;

class GuiScreen : public Gui {
public:
	Minecraft* mc;
	int width;
	int height;
	JList<GuiButton*> controlList;
	// CONSOLE (controller menu nav): D-pad-selected button index (-1 = none yet). CodeBreaker-injected.
	int navSelected;
	void handleControllerMenuNav();
	bool allowUserInput;

	GuiScreen();
	virtual void drawScreen(int x, int y, float a);
	virtual void updateScreen();
};

#endif // GUISCREEN_H
