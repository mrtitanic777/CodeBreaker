#ifndef GUIUNUSED_H
#define GUIUNUSED_H

// Auto-generated from GuiUnused.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiUnused : public GuiScreen {
public:
	GuiUnused();
	virtual ~GuiUnused();

	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);

public: // was protected
	virtual void keyTyped(char var1, int var2);

public: // was private
	std::string m_message1;
	std::string m_message2;

};

#endif // GUIUNUSED_H