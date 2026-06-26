#include "stdafx.h"
// Auto-generated from GuiCrafting.java  (handwritten override)
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiCrafting.h"
#include "Common.h"
#include "CraftingInventoryWorkbenchCB.h"
#include "InventoryPlayer.h"
#include "InventoryCrafting.h"
#include "SlotCrafting.h"
#include "Tesselator.h"
#include "Textures.h"
#include "FontRenderer.h"
#include "GuiContainer.h"
#include "InventoryCraftResult.h"
#include "Minecraft.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "SlotInventory.h"

GuiCrafting::GuiCrafting()
	: m_field_979_j(new CraftingInventoryWorkbenchCB())
{
}

GuiCrafting::GuiCrafting(InventoryPlayer* var1)
	: m_field_979_j(new CraftingInventoryWorkbenchCB())
{
		inventorySlots.push_back(new SlotCrafting(this, m_field_979_j->m_craftMatrix, m_field_979_j->m_craftResult, 0, 124, 35));

		int var2;
		int var3;
		for(var2 = 0; var2 < 3; ++var2) {
			for(var3 = 0; var3 < 3; ++var3) {
				inventorySlots.push_back(new SlotInventory(this, m_field_979_j->m_craftMatrix, var3 + var2 * 3, 30 + var3 * 18, 17 + var2 * 18));
			}
		}

		for(var2 = 0; var2 < 3; ++var2) {
			for(var3 = 0; var3 < 9; ++var3) {
				inventorySlots.push_back(new SlotInventory(this, var1, var3 + (var2 + 1) * 9, 8 + var3 * 18, 84 + var2 * 18));
			}
		}

		for(var2 = 0; var2 < 9; ++var2) {
			inventorySlots.push_back(new SlotInventory(this, var1, var2, 8 + var2 * 18, 142));
		}

}

GuiCrafting::~GuiCrafting() {
	m_field_979_j = NULL;
}

void GuiCrafting::onGuiClosed() {
		GuiContainer::onGuiClosed();
		m_field_979_j->onCraftGuiClosed((EntityPlayer*)mc->thePlayer);
}

void GuiCrafting::drawGuiContainerForegroundLayer() {
		fontRenderer->drawString("Crafting", 28, 6, 4210752);
		fontRenderer->drawString("Inventory", 8, m_ySize - 96 + 2, 4210752);
}

void GuiCrafting::drawGuiContainerBackgroundLayer(float var1) {
		int var2 = Textures::load("game:\\Content\\gui\\crafting.png");
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		g_pd3dDevice->SetTexture(0, Textures::getById(var2));
		int var3 = (width - m_xSize) / 2;
		int var4 = (height - m_ySize) / 2;
		drawTexturedModalRect(var3, var4, 0, 0, m_xSize, m_ySize);
}
