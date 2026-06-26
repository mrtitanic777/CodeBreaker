#ifndef GUIERRORSCREEN_H
#define GUIERRORSCREEN_H

// Auto-generated from GuiErrorScreen.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiErrorScreen : public GuiScreen {
public:
	GuiErrorScreen();
	GuiErrorScreen(const std::string& var1, const std::string& var2);
	virtual ~GuiErrorScreen();

	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void keyTyped(char var1, int var2);

public: // was private
	std::string m_title;
	std::string m_text;

};

#endif // GUIERRORSCREEN_H