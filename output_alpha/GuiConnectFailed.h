#ifndef GUICONNECTFAILED_H
#define GUICONNECTFAILED_H

// Auto-generated from GuiConnectFailed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiButton;

class GuiConnectFailed : public GuiScreen {
public:
	GuiConnectFailed();
	GuiConnectFailed(const std::string& var1, const std::string& var2);
	virtual ~GuiConnectFailed();

	virtual void updateScreen();
	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void keyTyped(char var1, int var2);
	virtual void actionPerformed(GuiButton* var1);

public: // was private
	std::string m_errorMessage;
	std::string m_errorDetail;

};

#endif // GUICONNECTFAILED_H