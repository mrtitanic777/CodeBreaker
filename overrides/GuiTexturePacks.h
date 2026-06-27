#ifndef GUITEXTUREPACKS_H
#define GUITEXTUREPACKS_H

// Auto-generated from GuiTexturePacks.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;

class GuiTexturePacks : public GuiScreen {
public:
	GuiTexturePacks(GuiScreen* var1);
	virtual ~GuiTexturePacks();

	GuiScreen* parentScreen;
	int scrollOffset;
	int listTop;
	int listBottom;
	int listLeft;
	int listRight;
	int mousePrevY;
	int refreshTimer;
	JavaString texturePackDir;

	virtual void initGui();
	virtual void actionPerformed(GuiButton* var1);
	virtual void mouseClicked(int var1, int var2, int var3);
	virtual void mouseMovedOrUp(int var1, int var2, int var3);
	virtual void drawScreen(int var1, int var2, float var3);
	virtual void updateScreen();
	virtual void drawGradientOverlay(int var1, int var2, int var3, int var4);
};

#endif // GUITEXTUREPACKS_H