#ifndef GUIBUTTON_H
#define GUIBUTTON_H

// Auto-generated from GuiButton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Gui.h"

class Minecraft;

class GuiButton : public Gui {
public:
	GuiButton(int var1, int var2, int var3, const JavaString& var4);
	GuiButton(int var1, int var2, int var3, int var4, int var5, const JavaString& var6);
	virtual ~GuiButton();

	int width;
	int height;
	int xPosition;
	int yPosition;
	JavaString displayString;
	int id;
	bool enabled;
	bool enabled2;

	virtual int getHoverState(bool var1);
	virtual void drawButton(Minecraft* var1, int var2, int var3);
	virtual void mouseDragged(Minecraft* var1, int var2, int var3);
	virtual void mouseReleased(int var1, int var2);
	virtual bool mousePressed(Minecraft* var1, int var2, int var3);

	static void initStatics();
};

#endif // GUIBUTTON_H