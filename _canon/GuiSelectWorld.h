#ifndef GUISELECTWORLD_H
#define GUISELECTWORLD_H

// Auto-generated from GuiSelectWorld.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;

class GuiSelectWorld : public GuiScreen {
public:
	GuiSelectWorld(GuiScreen* var1);
	virtual ~GuiSelectWorld();

	GuiScreen* parentScreen;
	JavaString screenTitle;
	bool selected;

	virtual void initGui();
	virtual JavaString getWorldName(int var1);
	virtual void initGui2();
	virtual void actionPerformed(GuiButton* var1);
	virtual void selectWorld(int var1);
	virtual void drawScreen(int var1, int var2, float var3);

	static void initStatics();
};

#endif // GUISELECTWORLD_H