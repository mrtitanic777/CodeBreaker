#ifndef GUICONTROLS_H
#define GUICONTROLS_H

// Auto-generated from GuiControls.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GameSettings;
class GuiButton;

class GuiControls : public GuiScreen {
public:
	GuiControls();
	GuiControls(GuiScreen* var1, GameSettings* var2);
	virtual ~GuiControls();

	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void actionPerformed(GuiButton* var1);
	virtual void keyTyped(char var1, int var2);

public: // was private
	GuiScreen* m_parentScreen;
	std::string m_screenTitle;
	GameSettings* m_options;
	int m_buttonId;

};

#endif // GUICONTROLS_H