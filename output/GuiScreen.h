#ifndef GUISCREEN_H
#define GUISCREEN_H

// Auto-generated from GuiScreen.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Gui.h"
#include <vector>

class FontRenderer;
class GuiButton;
class Minecraft;

class GuiScreen : public Gui {
public:
	GuiScreen();
	virtual ~GuiScreen();

	int width;
	int height;
	bool m_allowUserInput;

	virtual void drawScreen(int var1, int var2, float var3);
	virtual void setWorldAndResolution(Minecraft* var1, int var2, int var3);
	virtual void initGui();
	virtual void handleMouseInput();
	virtual void handleKeyboardInput();
	virtual void updateScreen();
	virtual void onGuiClosed();
	virtual void drawDefaultBackground();
	virtual bool doesGuiPauseGame();

public: // was protected
	Minecraft* mc;
	std::vector<GuiButton*> controlList;
	FontRenderer* fontRenderer;

	virtual void keyTyped(char var1, int var2);
	virtual void drawSlotInventory(int var1, int var2, int var3);
	virtual void actionPerformed(GuiButton* var1);

};

#endif // GUISCREEN_H