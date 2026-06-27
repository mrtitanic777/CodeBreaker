#ifndef GUICONNECTING_H
#define GUICONNECTING_H

// Auto-generated from GuiConnecting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;
class Minecraft;
class NetClientHandler;

class GuiConnecting : public GuiScreen {
public:
	GuiConnecting(Minecraft* var1, const JavaString& var2, int var3);
	virtual ~GuiConnecting();

	NetClientHandler* clientHandler;
	bool cancelled;

	virtual void updateScreen();
	virtual void keyTyped(char var1, int var2);
	virtual void initGui();
	virtual void actionPerformed(GuiButton* var1);
	virtual void drawScreen(int var1, int var2, float var3);
	static NetClientHandler* setNetClientHandler(GuiConnecting* var0, NetClientHandler* var1);
	static bool isCancelled(GuiConnecting* var0);
	static NetClientHandler* getNetClientHandler(GuiConnecting* var0);

	static void initStatics();
};

#endif // GUICONNECTING_H