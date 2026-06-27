#ifndef GUICHAT_H
#define GUICHAT_H

// Auto-generated from GuiChat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiChat : public GuiScreen {
public:
	GuiChat();
	virtual ~GuiChat();

	JavaString message;
	int updateCounter;

	virtual void initGui();
	virtual void onGuiClosed();
	virtual void updateScreen();
	virtual void keyTyped(char var1, int var2);
	virtual void drawScreen(int var1, int var2, float var3);
	virtual void mouseClicked(int var1, int var2, int var3);

	static void initStatics();
};

#endif // GUICHAT_H