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
	, m_id(0)
	, m_enabled(false)
	, m_visible(false)
{
}

GuiButton::GuiButton(int var1, int var2, int var3, const std::string& var4)
	: width(0)
	, height(0)
	, m_xPosition(0)
	, m_yPosition(0)
	, m_id(0)
	, m_enabled(false)
	, m_visible(false)
{
  // PORT: constructor delegation  // was: this(var1, var2, var3, 200, 20, var4);
}

GuiButton::GuiButton(int var1, int var2, int var3, int var4, int var5, const std::string& var6)
	: width(0)
	, height(0)
	, m_xPosition(0)
	, m_yPosition(0)
	, m_id(0)
	, m_enabled(false)
	, m_visible(false)
{
		width = 200;
		height = 20;
		m_enabled = true;
		m_visible = true;
		m_id = var1;
		m_xPosition = var2;
		m_yPosition = var3;
		width = var4;
		height = 20;
		m_displayString = var6;
}

GuiButton::~GuiButton() {
	// No dynamic resources
}

void GuiButton::drawButton(Minecraft* var1, int var2, int var3) {
		if(m_visible) {
			FontRenderer* var4 = var1->fontRenderer;
   g_pd3dDevice->SetTexture(0, Textures::getById(var1->m_renderEngine->getTexture("/gui/gui.png")));
   Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			unsigned char var5 = 1;
			bool var6 = var2 >= m_xPosition && var3 >= m_yPosition && var2 < m_xPosition + width && var3 < m_yPosition + height;
			if(!m_enabled) {
				var5 = 0;
			} else if(var6) {
				var5 = 2;
			}

			drawTexturedModalRect(m_xPosition, m_yPosition, 0, 46 + var5 * 20, width / 2, height);
			drawTexturedModalRect(m_xPosition + width / 2, m_yPosition, 200 - width / 2, 46 + var5 * 20, width / 2, height);
			if(!m_enabled) {
				drawCenteredString(var4, m_displayString, m_xPosition + width / 2, m_yPosition + (height - 8) / 2, -6250336);
			} else if(var6) {
				drawCenteredString(var4, m_displayString, m_xPosition + width / 2, m_yPosition + (height - 8) / 2, 16777120);
			} else {
				drawCenteredString(var4, m_displayString, m_xPosition + width / 2, m_yPosition + (height - 8) / 2, 14737632);
			}
		}
}

bool GuiButton::mousePressed(int var1, int var2) {
		return m_enabled && var1 >= m_xPosition && var2 >= m_yPosition && var1 < m_xPosition + width && var2 < m_yPosition + height;
}
