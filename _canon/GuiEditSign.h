#ifndef GUIEDITSIGN_H
#define GUIEDITSIGN_H

// Auto-generated from GuiEditSign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;
class TileEntitySign;

class GuiEditSign : public GuiScreen {
public:
	GuiEditSign(TileEntitySign* var1);
	virtual ~GuiEditSign();

	JavaString screenTitle;
	TileEntitySign* entitySign;
	int updateCounter;
	int editLine;

	virtual void initGui();
	virtual void onGuiClosed();
	virtual void updateScreen();
	virtual void actionPerformed(GuiButton* var1);
	virtual void keyTyped(char var1, int var2);
	virtual void drawScreen(int var1, int var2, float var3);

	static void initStatics();
};

#endif // GUIEDITSIGN_H