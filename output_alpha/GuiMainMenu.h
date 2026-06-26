#ifndef GUIMAINMENU_H
#define GUIMAINMENU_H

// Auto-generated from GuiMainMenu.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiButton;
class LogoEffectRandomizer;
class Random;

class GuiMainMenu : public GuiScreen {
public:
	GuiMainMenu();
	virtual ~GuiMainMenu();

	virtual void updateScreen();
	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void keyTyped(char var1, int var2);
	virtual void actionPerformed(GuiButton* var1);

public: // was private
	static Random* s_rand;
	std::string* m_minecraftLogo;
	LogoEffectRandomizer** m_logoEffects;
	float m_updateCounter;
	std::string m_splashText;

	void drawLogo(float var1);
	Random* getRand();

public:  // array-length companions
	int m_minecraftLogo_count;
	int m_logoEffects_count;

};

#endif // GUIMAINMENU_H