#ifndef GUIINGAMEMENU_H
#define GUIINGAMEMENU_H

// Auto-generated from GuiIngameMenu.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;

class GuiIngameMenu : public GuiScreen {
public:
	GuiIngameMenu();
	virtual ~GuiIngameMenu();

	int updateCounter2;
	int updateCounter;

	virtual void initGui();
	virtual void actionPerformed(GuiButton* var1);
	virtual void updateScreen();
	virtual void drawScreen(int var1, int var2, float var3);

	static void initStatics();
};

#endif // GUIINGAMEMENU_H