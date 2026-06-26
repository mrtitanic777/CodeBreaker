#ifndef GUICONFLICTWARNING_H
#define GUICONFLICTWARNING_H

// Auto-generated from GuiConflictWarning.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"

class GuiButton;

class GuiConflictWarning : public GuiScreen {
public:
	GuiConflictWarning();
	virtual ~GuiConflictWarning();

	virtual void updateScreen();
	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void actionPerformed(GuiButton* var1);

public: // was private
	int m_updateCounter;

};

#endif // GUICONFLICTWARNING_H