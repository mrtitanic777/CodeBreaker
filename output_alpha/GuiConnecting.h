#ifndef GUICONNECTING_H
#define GUICONNECTING_H

// Auto-generated from GuiConnecting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiButton;
class Minecraft;
class NetClientHandler;

class GuiConnecting : public GuiScreen {
public:
	GuiConnecting();
	GuiConnecting(Minecraft* var1, const std::string& var2, int var3);
	virtual ~GuiConnecting();

	virtual void updateScreen();
	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void keyTyped(char var1, int var2);
	virtual void actionPerformed(GuiButton* var1);

public: // was private
	NetClientHandler* m_clientHandler;
	bool m_cancelled;

	NetClientHandler* setNetClientHandler(GuiConnecting* var0, NetClientHandler* var1);
	bool isCancelled(GuiConnecting* var0);
	NetClientHandler* getNetClientHandler(GuiConnecting* var0);

};

#endif // GUICONNECTING_H