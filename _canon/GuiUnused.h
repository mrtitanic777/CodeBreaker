#ifndef GUIUNUSED_H
#define GUIUNUSED_H

// Auto-generated from GuiUnused.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiUnused : public GuiScreen {
public:
	virtual ~GuiUnused();

	JavaString message1;
	JavaString message2;

	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);
	virtual void keyTyped(char var1, int var2);

	static void initStatics();
};

#endif // GUIUNUSED_H