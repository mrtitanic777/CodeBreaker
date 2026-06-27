#ifndef GUI_H
#define GUI_H

// Auto-generated from Gui.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class FontRenderer;

class Gui {
public:
	Gui();
	virtual ~Gui();

	float zLevel;

	virtual void drawRect(int var1, int var2, int var3, int var4, int var5);
	virtual void drawGradientRect(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual void drawCenteredString(FontRenderer* var1, const JavaString& var2, int var3, int var4, int var5);
	virtual void drawString(FontRenderer* var1, const JavaString& var2, int var3, int var4, int var5);
	virtual void drawTexturedModalRect(int var1, int var2, int var3, int var4, int var5, int var6);

	static void initStatics();
};

#endif // GUI_H