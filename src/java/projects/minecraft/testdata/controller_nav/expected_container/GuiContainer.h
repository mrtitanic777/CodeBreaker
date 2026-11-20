#ifndef GUICONTAINER_H
#define GUICONTAINER_H

#include "GuiScreen.h"

class GuiContainer : public GuiScreen {
public:
	virtual void drawScreen(int x, int y, float a);
};

#endif // GUICONTAINER_H
