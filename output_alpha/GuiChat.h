#ifndef GUICHAT_H
#define GUICHAT_H

// Auto-generated from GuiChat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiChat : public GuiScreen {
public:
	GuiChat();
	virtual ~GuiChat();

	virtual void initGui();
	virtual void onGuiClosed();
	virtual void updateScreen();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void keyTyped(char var1, int var2);
	virtual void mouseClicked(int var1, int var2, int var3);

public: // was private
	std::string m_message;
	int m_updateCounter;

};

#endif // GUICHAT_H