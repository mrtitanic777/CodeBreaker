#ifndef GUIOPTIONS_H
#define GUIOPTIONS_H

// Auto-generated from GuiOptions.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GameSettings;
class GuiButton;

class GuiOptions : public GuiScreen {
public:
	GuiOptions();
	GuiOptions(GuiScreen* var1, GameSettings* var2);
	virtual ~GuiOptions();

	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

protected:
	virtual void actionPerformed(GuiButton* var1);

private:
	GuiScreen* m_parentScreen;
	std::string m_screenTitle;
	GameSettings* m_options;

};

#endif // GUIOPTIONS_H