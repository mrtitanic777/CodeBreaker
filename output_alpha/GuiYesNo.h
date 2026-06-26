#ifndef GUIYESNO_H
#define GUIYESNO_H

// Auto-generated from GuiYesNo.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiButton;

class GuiYesNo : public GuiScreen {
public:
	GuiYesNo();
	GuiYesNo(GuiScreen* var1, const std::string& var2, const std::string& var3, int var4);
	virtual ~GuiYesNo();

	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void actionPerformed(GuiButton* var1);

public: // was private
	GuiScreen* m_parentScreen;
	std::string m_message1;
	std::string m_message2;
	int m_worldNumber;

};

#endif // GUIYESNO_H