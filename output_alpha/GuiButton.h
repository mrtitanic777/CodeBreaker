#ifndef GUIBUTTON_H
#define GUIBUTTON_H

// Auto-generated from GuiButton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Gui.h"
#include <string>

class Minecraft;

class GuiButton : public Gui {
public:
	GuiButton();
	GuiButton(int var1, int var2, int var3, const std::string& var4);
	GuiButton(int var1, int var2, int var3, int var4, int var5, const std::string& var6);
	virtual ~GuiButton();

	int m_xPosition;
	int m_yPosition;
	std::string m_displayString;
	int m_id;
	bool m_enabled;
	bool m_enabled2;

	virtual void drawButton(Minecraft* var1, int var2, int var3);
	virtual void mouseReleased(int var1, int var2);
	virtual bool mousePressed(Minecraft* var1, int var2, int var3);

public: // was protected
	int width;
	int height;

	virtual int getHoverState(bool var1);
	virtual void mouseDragged(Minecraft* var1, int var2, int var3);

};

#endif // GUIBUTTON_H