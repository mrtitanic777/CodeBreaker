#ifndef GUIGAMEOVER_H
#define GUIGAMEOVER_H

// Auto-generated from GuiGameOver.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;

class GuiGameOver : public GuiScreen {
public:
	virtual ~GuiGameOver();

	virtual void initGui();
	virtual void keyTyped(char var1, int var2);
	virtual void actionPerformed(GuiButton* var1);
	virtual void drawScreen(int var1, int var2, float var3);
	virtual bool doesGuiPauseGame();

	static void initStatics();
};

#endif // GUIGAMEOVER_H