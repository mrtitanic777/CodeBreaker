#ifndef GUIMULTIPLAYER_H
#define GUIMULTIPLAYER_H

// Auto-generated from GuiMultiplayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;

class GuiMultiplayer : public GuiScreen {
public:
	GuiMultiplayer(GuiScreen* var1);
	virtual ~GuiMultiplayer();

	GuiScreen* updateCounter;
	int parentScreen;
	JavaString serverAddress;

	virtual void updateScreen();
	virtual void initGui();
	virtual void actionPerformed(GuiButton* var1);
	virtual int getBiomeAt(const JavaString& var1, int var2);
	virtual void keyTyped(char var1, int var2);
	virtual void drawScreen(int var1, int var2, float var3);

	static void initStatics();
};

#endif // GUIMULTIPLAYER_H