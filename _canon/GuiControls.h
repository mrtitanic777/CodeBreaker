#ifndef GUICONTROLS_H
#define GUICONTROLS_H

// Auto-generated from GuiControls.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GameSettings;
class GuiButton;

class GuiControls : public GuiScreen {
public:
	GuiControls(GuiScreen* var1, GameSettings* var2);
	virtual ~GuiControls();

	GuiScreen* parentScreen;
	JavaString screenTitle;
	GameSettings* options;
	int buttonId;

	virtual void initGui();
	virtual void actionPerformed(GuiButton* var1);
	virtual void keyTyped(char var1, int var2);
	virtual void drawScreen(int var1, int var2, float var3);

	static void initStatics();
};

#endif // GUICONTROLS_H