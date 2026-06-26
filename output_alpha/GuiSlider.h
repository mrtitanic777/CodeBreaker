#ifndef GUISLIDER_H
#define GUISLIDER_H

// Auto-generated from GuiSlider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiButton.h"
#include <string>

class Minecraft;

class GuiSlider : public GuiButton {
public:
	GuiSlider();
	GuiSlider(int var1, int var2, int var3, int var4, const std::string& var5, float var6);
	virtual ~GuiSlider();

	float m_sliderValue;
	bool m_dragging;

	virtual bool mousePressed(Minecraft* var1, int var2, int var3);
	virtual void mouseReleased(int var1, int var2);

public: // was protected
	virtual int getHoverState(bool var1);
	virtual void mouseDragged(Minecraft* var1, int var2, int var3);

public: // was private
	int m_idFloat;

};

#endif // GUISLIDER_H