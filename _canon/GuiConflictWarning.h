#ifndef GUICONFLICTWARNING_H
#define GUICONFLICTWARNING_H

// Auto-generated from GuiConflictWarning.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiScreen.h"

class GuiButton;

class GuiConflictWarning : public GuiScreen {
public:
	GuiConflictWarning();
	virtual ~GuiConflictWarning();

	int updateCounter;

	virtual void updateScreen();
	virtual void initGui();
	virtual void actionPerformed(GuiButton* var1);
	virtual void drawScreen(int var1, int var2, float var3);

	static void initStatics();
};

#endif // GUICONFLICTWARNING_H