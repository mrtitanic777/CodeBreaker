#ifndef GUICONNECTFAILED_H
#define GUICONNECTFAILED_H

// Auto-generated from GuiConnectFailed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;

class GuiConnectFailed : public GuiScreen {
public:
	GuiConnectFailed(const JavaString& var1, const JavaString& var2);
	virtual ~GuiConnectFailed();

	JavaString errorMessage;
	JavaString errorDetail;

	virtual void updateScreen();
	virtual void keyTyped(char var1, int var2);
	virtual void initGui();
	virtual void actionPerformed(GuiButton* var1);
	virtual void drawScreen(int var1, int var2, float var3);

	static void initStatics();
};

#endif // GUICONNECTFAILED_H