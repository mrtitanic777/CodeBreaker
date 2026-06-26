#ifndef GUIEDITSIGN_H
#define GUIEDITSIGN_H

// Auto-generated from GuiEditSign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiButton;
class TileEntitySign;

class GuiEditSign : public GuiScreen {
public:
	GuiEditSign();
	GuiEditSign(TileEntitySign* var1);
	virtual ~GuiEditSign();

	virtual void initGui();
	virtual void onGuiClosed();
	virtual void updateScreen();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	std::string m_screenTitle;

	virtual void actionPerformed(GuiButton* var1);
	virtual void keyTyped(char var1, int var2);

public: // was private
	TileEntitySign* m_entitySign;
	int m_updateCounter;
	int m_editLine;

};

#endif // GUIEDITSIGN_H