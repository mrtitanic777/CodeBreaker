#ifndef GUISLIDER_H
#define GUISLIDER_H

// Auto-generated from GuiSlider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiButton.h"

class Minecraft;

class GuiSlider : public GuiButton {
public:
	GuiSlider(int var1, int var2, int var3, int var4, const JavaString& var5, float var6);
	virtual ~GuiSlider();

	float sliderValue;
	bool dragging;
	int idFloat;

	virtual int getHoverState(bool var1);
	virtual void mouseDragged(Minecraft* var1, int var2, int var3);
	virtual bool mousePressed(Minecraft* var1, int var2, int var3);
	virtual void mouseReleased(int var1, int var2);

	static void initStatics();
};

#endif // GUISLIDER_H