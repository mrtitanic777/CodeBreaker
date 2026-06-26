#ifndef GUIMAINMENU_H
#define GUIMAINMENU_H

// Auto-generated from GuiMainMenu.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiButton;

class GuiMainMenu : public GuiScreen {
public:
	GuiMainMenu();
	virtual ~GuiMainMenu();

	virtual void updateScreen();
	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

protected:
	virtual void keyTyped(char var1, int var2);
	virtual void actionPerformed(GuiButton* var1);

private:
	float m_updateCounter;
	std::string m_splashes;
	std::string m_currentSplash;

};

#endif // GUIMAINMENU_H