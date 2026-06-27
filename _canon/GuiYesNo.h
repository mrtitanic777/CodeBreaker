#ifndef GUIYESNO_H
#define GUIYESNO_H

// Auto-generated from GuiYesNo.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;

class GuiYesNo : public GuiScreen {
public:
	GuiYesNo(GuiScreen* var1, const JavaString& var2, const JavaString& var3, int var4);
	virtual ~GuiYesNo();

	GuiScreen* parentScreen;
	JavaString message1;
	JavaString message2;
	int worldNumber;

	virtual void initGui();
	virtual void actionPerformed(GuiButton* var1);
	virtual void drawScreen(int var1, int var2, float var3);

	static void initStatics();
};

#endif // GUIYESNO_H