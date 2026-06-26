#ifndef GUI_H
#define GUI_H

// Auto-generated from Gui.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class FontRenderer;

class Gui {
public:
	Gui();
	virtual ~Gui();

	virtual void drawCenteredString(FontRenderer* var1, const std::string& var2, int var3, int var4, int var5);
	virtual void drawString(FontRenderer* var1, const std::string& var2, int var3, int var4, int var5);
	virtual void drawTexturedModalRect(int var1, int var2, int var3, int var4, int var5, int var6);

public: // was protected
	float zLevel;

	virtual void drawRect(int var1, int var2, int var3, int var4, int var5);
	virtual void drawGradientRect(int var1, int var2, int var3, int var4, int var5, int var6);

};

#endif // GUI_H