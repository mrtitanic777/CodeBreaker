#include "stdafx.h"
// Auto-generated from GuiSlider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiSlider.h"
#include "Common.h"
#include "Minecraft.h"
#include <string>
#include "Tesselator.h"
#include "GameSettings.h"
#include "GuiButton.h"

GuiSlider::GuiSlider()
	: m_sliderValue(1.0f)
	, m_dragging(false)
	, m_idFloat(0)
{
}

GuiSlider::GuiSlider(int var1, int var2, int var3, int var4, const std::string& var5, float var6)
	: GuiButton(var1, var2, var3, 150, 20, var5)
	, m_sliderValue(1.0f)
	, m_dragging(false)
	, m_idFloat(0)
{
		m_idFloat = var4;
		m_sliderValue = var6;
}

GuiSlider::~GuiSlider() {
	// No dynamic resources
}

int GuiSlider::getHoverState(bool var1) {
		return 0;
}

void GuiSlider::mouseDragged(Minecraft* var1, int var2, int var3) {
		if(m_enabled2) {
			if(m_dragging) {
				m_sliderValue = (float)(var2 - (m_xPosition + 4)) / (float)(width - 8);
				if(m_sliderValue < 0.0F) {
					m_sliderValue = 0.0F;
				}

				if(m_sliderValue > 1.0F) {
					m_sliderValue = 1.0F;
				}

				var1->m_gameSettings->setOptionFloatValue(m_idFloat, m_sliderValue);
				m_displayString = var1->m_gameSettings->getOptionDisplayString(m_idFloat);
			}

   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			drawTexturedModalRect(m_xPosition + (int)(m_sliderValue * (float)(width - 8)), m_yPosition, 0, 66, 4, 20);
			drawTexturedModalRect(m_xPosition + (int)(m_sliderValue * (float)(width - 8)) + 4, m_yPosition, 196, 66, 4, 20);
		}
}

bool GuiSlider::mousePressed(Minecraft* var1, int var2, int var3) {
		if(GuiButton::mousePressed(var1, var2, var3)) {
			m_sliderValue = (float)(var2 - (m_xPosition + 4)) / (float)(width - 8);
			if(m_sliderValue < 0.0F) {
				m_sliderValue = 0.0F;
			}

			if(m_sliderValue > 1.0F) {
				m_sliderValue = 1.0F;
			}

			var1->m_gameSettings->setOptionFloatValue(m_idFloat, m_sliderValue);
			m_displayString = var1->m_gameSettings->getOptionDisplayString(m_idFloat);
			m_dragging = true;
			return true;
		} else {
			return false;
		}
}

void GuiSlider::mouseReleased(int var1, int var2) {
		m_dragging = false;
}
