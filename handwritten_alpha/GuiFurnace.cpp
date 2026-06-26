#include "stdafx.h"
// Auto-generated from GuiFurnace.java  (handwritten override)
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiFurnace.h"
#include "Common.h"
#include "InventoryPlayer.h"
#include "TileEntityFurnace.h"
#include "Tesselator.h"
#include "Textures.h"
#include "FontRenderer.h"
#include "Minecraft.h"
#include "SlotInventory.h"

GuiFurnace::GuiFurnace()
	: m_field_978_j(NULL)
{
}

GuiFurnace::GuiFurnace(InventoryPlayer* var1, TileEntityFurnace* var2)
	: m_field_978_j(NULL)
{
		m_field_978_j = var2;
		inventorySlots.push_back(new SlotInventory(this, var2, 0, 56, 17));
		inventorySlots.push_back(new SlotInventory(this, var2, 1, 56, 53));
		inventorySlots.push_back(new SlotInventory(this, var2, 2, 116, 35));

		int var3;
		for(var3 = 0; var3 < 3; ++var3) {
			for(int var4 = 0; var4 < 9; ++var4) {
				inventorySlots.push_back(new SlotInventory(this, var1, var4 + (var3 + 1) * 9, 8 + var4 * 18, 84 + var3 * 18));
			}
		}

		for(var3 = 0; var3 < 9; ++var3) {
			inventorySlots.push_back(new SlotInventory(this, var1, var3, 8 + var3 * 18, 142));
		}

}

GuiFurnace::~GuiFurnace() {
	m_field_978_j = NULL;
}

void GuiFurnace::drawGuiContainerForegroundLayer() {
		fontRenderer->drawString("Furnace", 60, 6, 4210752);
		fontRenderer->drawString("Inventory", 8, m_ySize - 96 + 2, 4210752);
}

void GuiFurnace::drawGuiContainerBackgroundLayer(float var1) {
		int var2 = Textures::load("game:\\Content\\gui\\furnace.png");
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		g_pd3dDevice->SetTexture(0, Textures::getById(var2));
		int var3 = (width - m_xSize) / 2;
		int var4 = (height - m_ySize) / 2;
		drawTexturedModalRect(var3, var4, 0, 0, m_xSize, m_ySize);
		int var5;
		if(m_field_978_j->isBurning()) {
			var5 = m_field_978_j->getBurnTimeRemainingScaled(12);
			drawTexturedModalRect(var3 + 56, var4 + 36 + 12 - var5, 176, 12 - var5, 14, var5 + 2);
		}

		var5 = m_field_978_j->getCookProgressScaled(24);
		drawTexturedModalRect(var3 + 79, var4 + 34, 176, 14, var5 + 1, 16);
}
