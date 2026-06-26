#ifndef GUISCREEN_H
#define GUISCREEN_H

// Auto-generated from GuiScreen.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Gui.h"
#include <string>
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
	bool m_field_948_f;

	virtual void drawScreen(int var1, int var2, float var3);
	static const std::string& getClipboardString();
	virtual void setWorldAndResolution(Minecraft* var1, int var2, int var3);
	virtual void initGui();
	virtual void handleInput();
	virtual void handleMouseInput();
	virtual void handleKeyboardInput();
	virtual void updateScreen();
	virtual void onGuiClosed();
	virtual void drawDefaultBackground();
	virtual void func_567_a(int var1);
	virtual void drawBackground(int var1);
	virtual bool doesGuiPauseGame();
	virtual void deleteWorld(bool var1, int var2);

public: // was protected
	Minecraft* mc;
	std::vector<GuiButton*> controlList;
	FontRenderer* fontRenderer;

	virtual void keyTyped(char var1, int var2);
	virtual void mouseClicked(int var1, int var2, int var3);
	virtual void mouseMovedOrUp(int var1, int var2, int var3);
	virtual void actionPerformed(GuiButton* var1);

public: // was private
	GuiButton* m_selectedButton;

};

#endif // GUISCREEN_H