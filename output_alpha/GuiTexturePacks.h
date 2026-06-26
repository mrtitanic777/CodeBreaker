#ifndef GUITEXTUREPACKS_H
#define GUITEXTUREPACKS_H

// Auto-generated from GuiTexturePacks.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiScreen.h"
#include <string>

class GuiButton;

class GuiTexturePacks : public GuiScreen {
public:
	GuiTexturePacks();
	GuiTexturePacks(GuiScreen* var1);
	virtual ~GuiTexturePacks();

	virtual void initGui();
	virtual void drawScreen(int var1, int var2, float var3);
	virtual void updateScreen();
	virtual void func_6452_a(int var1, int var2, int var3, int var4);

public: // was protected
	GuiScreen* m_field_6461_a;

	virtual void actionPerformed(GuiButton* var1);
	virtual void mouseClicked(int var1, int var2, int var3);
	virtual void mouseMovedOrUp(int var1, int var2, int var3);

public: // was private
	int m_field_6460_h;
	int m_field_6459_i;
	int m_field_6458_j;
	int m_field_6457_l;
	int m_field_6456_m;
	int m_field_6455_n;
	int m_field_6454_o;
	std::string m_field_6453_p;

};

#endif // GUITEXTUREPACKS_H