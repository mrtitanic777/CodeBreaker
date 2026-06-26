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

	std::string m_displayString;
	int m_id;
	bool m_enabled;
	bool m_visible;

	virtual void drawButton(Minecraft* var1, int var2, int var3);
	virtual bool mousePressed(int var1, int var2);

protected:

private:
	int width;
	int height;
	int m_xPosition;
	int m_yPosition;

};

#endif // GUIBUTTON_H