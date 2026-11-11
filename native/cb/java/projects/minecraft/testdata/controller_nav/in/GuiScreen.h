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
	bool allowUserInput;

	GuiScreen();
	virtual void drawScreen(int x, int y, float a);
	virtual void updateScreen();
};

#endif // GUISCREEN_H
