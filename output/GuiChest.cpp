#include "stdafx.h"
// Auto-generated from GuiChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiChest.h"
#include "Common.h"
#include "IInventory.h"
#include "Slot.h"
#include "Tesselator.h"
#include "Textures.h"
#include "FontRenderer.h"
#include "Minecraft.h"
#include "RenderEngine.h"

GuiChest::GuiChest()
	: m_upperChestInventory(NULL)
	, m_lowerChestInventory(NULL)
	, m_inventoryRows(0)
{
}

GuiChest::GuiChest(IInventory* var1, IInventory* var2)
	: m_upperChestInventory(NULL)
	, m_lowerChestInventory(NULL)
	, m_inventoryRows(0)
{
		m_upperChestInventory = var1;
		m_lowerChestInventory = var2;
		m_allowUserInput = false;
		m_inventoryRows = var2->getSizeInventory() / 9;
		m_ySize = 114 + m_inventoryRows * 18;
		int var3 = (m_inventoryRows - 4) * 18;

		int var4;
		int var5;
		for(var4 = 0; var4 < m_inventoryRows; ++var4) {
			for(var5 = 0; var5 < 9; ++var5) {
				inventorySlots.push_back(new Slot(this, var2, var5 + var4 * 9, 8 + var5 * 18, 18 + var4 * 18));
			}
		}

		for(var4 = 0; var4 < 3; ++var4) {
			for(var5 = 0; var5 < 9; ++var5) {
				inventorySlots.push_back(new Slot(this, var1, var5 + (var4 + 1) * 9, 8 + var5 * 18, 103 + var4 * 18 + var3));
			}
		}

		for(var4 = 0; var4 < 9; ++var4) {
			inventorySlots.push_back(new Slot(this, var1, var4, 8 + var4 * 18, var3 + 161));
		}

}

GuiChest::~GuiChest() {
	m_upperChestInventory = NULL;
	m_lowerChestInventory = NULL;
}

void GuiChest::drawGuiContainerForegroundLayer() {
		fontRenderer->drawString(m_lowerChestInventory->getInvName(), 8, 6, 4210752);
		fontRenderer->drawString(m_upperChestInventory->getInvName(), 8, m_ySize - 96 + 2, 4210752);
}

void GuiChest::drawGuiContainerBackgroundLayer() {
		int var1 = Textures::load("game:\Content\gui\container.png");
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		RenderEngine::bindTexture(var1);
		var1 = (width - m_xSize) / 2;
		int var2 = (height - m_ySize) / 2;
		drawTexturedModalRect(var1, var2, 0, 0, m_xSize, m_inventoryRows * 18 + 17);
		drawTexturedModalRect(var1, var2 + m_inventoryRows * 18 + 17, 0, 126, m_xSize, 96);
}
