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

	static void drawCenteredString(FontRenderer* var0, const std::string& var1, int var2, int var3, int var4);
	static void drawString(FontRenderer* var0, const std::string& var1, int var2, int var3, int var4);
	virtual void drawTexturedModalRect(int var1, int var2, int var3, int var4, int var5, int var6);

protected:
	float zLevel;

	void drawRect(int var0, int var1, int var2, int var3, int var4);
	void drawGradientRect(int var0, int var1, int var2, int var3, int var4, int var5);

};

#endif // GUI_H