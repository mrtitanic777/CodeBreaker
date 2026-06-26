#include "stdafx.h"
// Auto-generated from GuiButton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiButton.h"
#include "Common.h"
#include "Minecraft.h"
#include <string>
#include "FontRenderer.h"
#include "Tesselator.h"
#include "Textures.h"
#include "RenderEngine.h"

GuiButton::GuiButton()
	: width(0)
	, height(0)
	, m_xPosition(0)
	, m_yPosition(0)
	, m_displayString("")
	, m_id(0)
	, m_enabled(false)
	, m_enabled2(false)
{
}

GuiButton::GuiButton(int var1, int var2, int var3, const std::string& var4)
	: width(0)
	, height(0)
	, m_xPosition(0)
	, m_yPosition(0)
	, m_displayString("")
	, m_id(0)
	, m_enabled(false)
	, m_enabled2(false)
{
  // PORT: constructor delegation  // was: this(var1, var2, var3, 200, 20, var4);
}

GuiButton::GuiButton(int var1, int var2, int var3, int var4, int var5, const std::string& var6)
	: width(0)
	, height(0)
	, m_xPosition(0)
	, m_yPosition(0)
	, m_displayString("")
	, m_id(0)
	, m_enabled(false)
	, m_enabled2(false)
{
		width = 200;
		height = 20;
		m_enabled = true;
		m_enabled2 = true;
		m_id = var1;
		m_xPosition = var2;
		m_yPosition = var3;
		width = var4;
		height = var5;
		m_displayString = var6;
}

GuiButton::~GuiButton() {
	// No dynamic resources
}

int GuiButton::getHoverState(bool var1) {
		unsigned char var2 = 1;
		if(!m_enabled) {
			var2 = 0;
		} else if(var1) {
			var2 = 2;
		}

		return var2;
}

void GuiButton::drawButton(Minecraft* var1, int var2, int var3) {
		if(m_enabled2) {
			FontRenderer* var4 = var1->fontRenderer;
   g_pd3dDevice->SetTexture(0, Textures::getById(var1->m_renderEngine->getTexture("/gui/gui.png")));
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			bool var5 = var2 >= m_xPosition && var3 >= m_yPosition && var2 < m_xPosition + width && var3 < m_yPosition + height;
			int var6 = getHoverState(var5);
			drawTexturedModalRect(m_xPosition, m_yPosition, 0, 46 + var6 * 20, width / 2, height);
			drawTexturedModalRect(m_xPosition + width / 2, m_yPosition, 200 - width / 2, 46 + var6 * 20, width / 2, height);
			mouseDragged(var1, var2, var3);
			if(!m_enabled) {
				drawCenteredString(var4, m_displayString, m_xPosition + width / 2, m_yPosition + (height - 8) / 2, -6250336);
			} else if(var5) {
				drawCenteredString(var4, m_displayString, m_xPosition + width / 2, m_yPosition + (height - 8) / 2, 16777120);
			} else {
				drawCenteredString(var4, m_displayString, m_xPosition + width / 2, m_yPosition + (height - 8) / 2, 14737632);
			}

		}
}

void GuiButton::mouseDragged(Minecraft* var1, int var2, int var3) {
	// TODO: implement
}

void GuiButton::mouseReleased(int var1, int var2) {
	// TODO: implement
}

bool GuiButton::mousePressed(Minecraft* var1, int var2, int var3) {
		return m_enabled && var2 >= m_xPosition && var3 >= m_yPosition && var2 < m_xPosition + width && var3 < m_yPosition + height;
}
