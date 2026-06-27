#ifndef GUISCREEN_H
#define GUISCREEN_H

// Auto-generated from GuiScreen.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Gui.h"

class FontRenderer;
class GuiButton;
class Minecraft;

class GuiScreen : public Gui {
public:
	// Java zero-inits all fields; C++ does NOT. With no ctor, allowUserInput was garbage -> the
	// in-world input block in Minecraft::runTick (gated on allowUserInput) ran on the MENU and
	// dereferenced the NULL thePlayer/playerController -> IP 0x0. Initialize everything to Java defaults.
	GuiScreen() : mc(0), width(0), height(0), allowUserInput(false), fontRenderer(0), selectedButton(0) {}
	virtual ~GuiScreen();

	Minecraft* mc;
	int width;
	int height;
	JList<void*> controlList;
	bool allowUserInput;
	FontRenderer* fontRenderer;
	GuiButton* selectedButton;

	virtual void drawScreen(int var1, int var2, float var3);
	virtual void keyTyped(char var1, int var2);
	static JavaString getClipboardString();
	virtual void mouseClicked(int var1, int var2, int var3);
	virtual void mouseMovedOrUp(int var1, int var2, int var3);
	virtual void actionPerformed(GuiButton* var1);
	virtual void setWorldAndResolution(Minecraft* var1, int var2, int var3);
	virtual void initGui();
	virtual void handleInput();
	virtual void handleMouseInput();
	virtual void handleKeyboardInput();
	virtual void updateScreen();
	virtual void onGuiClosed();
	virtual void drawDefaultBackground();
	virtual void createEntityInWorld(int var1);
	virtual void drawBackground(int var1);
	virtual bool doesGuiPauseGame();
	virtual void deleteWorld(bool var1, int var2);
};

#endif // GUISCREEN_H