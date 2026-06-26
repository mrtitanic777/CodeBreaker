#ifndef GUIINGAMEMENU_H
#define GUIINGAMEMENU_H

// Auto-generated from GuiIngameMenu.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"

class GuiButton;

class GuiIngameMenu : public GuiScreen {
public:
	GuiIngameMenu();
	virtual ~GuiIngameMenu();

	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void actionPerformed(GuiButton* var1);

};

#endif // GUIINGAMEMENU_H