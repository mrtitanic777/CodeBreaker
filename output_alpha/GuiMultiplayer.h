#ifndef GUIMULTIPLAYER_H
#define GUIMULTIPLAYER_H

// Auto-generated from GuiMultiplayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiButton;

class GuiMultiplayer : public GuiScreen {
public:
	GuiMultiplayer();
	GuiMultiplayer(GuiScreen* var1);
	virtual ~GuiMultiplayer();

	virtual void updateScreen();
	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void actionPerformed(GuiButton* var1);
	virtual void keyTyped(char var1, int var2);

public: // was private
	GuiScreen* m_updateCounter;
	int m_parentScreen;
	std::string m_serverAddress;

	int func_4067_a(const std::string& var1, int var2);

};

#endif // GUIMULTIPLAYER_H