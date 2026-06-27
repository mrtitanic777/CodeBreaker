#ifndef GUIOPTIONS_H
#define GUIOPTIONS_H

// Auto-generated from GuiOptions.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GameSettings;
class GuiButton;

class GuiOptions : public GuiScreen {
public:
	GuiOptions(GuiScreen* var1, GameSettings* var2);
	virtual ~GuiOptions();

	GuiScreen* parentScreen;
	JavaString screenTitle;
	GameSettings* options;

	virtual void initGui();
	virtual void actionPerformed(GuiButton* var1);
	virtual void drawScreen(int var1, int var2, float var3);

	static void initStatics();
};

#endif // GUIOPTIONS_H