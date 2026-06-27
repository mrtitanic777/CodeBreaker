#ifndef GUIMAINMENU_H
#define GUIMAINMENU_H

// Auto-generated from GuiMainMenu.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;
class LogoEffectRandomizer;
class Random;

class GuiMainMenu : public GuiScreen {
public:
	GuiMainMenu();
	virtual ~GuiMainMenu();

	JArray<JavaString > minecraftLogo;
	JArray<JArray<LogoEffectRandomizer* > > logoEffects;
	float updateCounter;
	JavaString splashText;
	static Random* s_rand;

	virtual void updateScreen();
	virtual void keyTyped(char var1, int var2);
	virtual void initGui();
	virtual void actionPerformed(GuiButton* var1);
	virtual void drawScreen(int var1, int var2, float var3);
	virtual void drawLogo(float var1);
	static Random* getRand();
};

#endif // GUIMAINMENU_H