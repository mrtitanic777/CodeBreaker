#include "stdafx.h"
// Auto-generated from GuiCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiCrafting.h"
#include "Common.h"
#include "IInventory.h"
#include "InventoryCrafting.h"
#include "InventoryPlayer.h"
#include "CraftingManager.h"
#include "ItemStack.h"
#include "Slot.h"
#include "SlotCrafting.h"
#include "Tesselator.h"
#include "Textures.h"
#include "FontRenderer.h"
#include "InventoryCraftResult.h"
#include "Minecraft.h"

GuiCrafting::GuiCrafting()
	: m_inventoryCrafting(new InventoryCrafting(this, 3, 3))
	, m_iInventory(new InventoryCraftResult())
{
}

GuiCrafting::GuiCrafting(InventoryPlayer* var1)
	: m_inventoryCrafting(new InventoryCrafting(this, 3, 3))
	, m_iInventory(new InventoryCraftResult())
{
		inventorySlots.push_back(SlotCrafting(this, m_inventoryCrafting, m_iInventory, 0, 124, 35));

		int var2;
		int var3;
		for(var2 = 0; var2 < 3; ++var2) {
			for(var3 = 0; var3 < 3; ++var3) {
				inventorySlots.push_back(Slot(this, m_inventoryCrafting, var3 + var2 * 3, 30 + var3 * 18, 17 + var2 * 18));
			}
		}

		for(var2 = 0; var2 < 3; ++var2) {
			for(var3 = 0; var3 < 9; ++var3) {
				inventorySlots.push_back(Slot(this, var1, var3 + (var2 + 1) * 9, 8 + var3 * 18, 84 + var2 * 18));
			}
		}

		for(var2 = 0; var2 < 9; ++var2) {
			inventorySlots.push_back(Slot(this, var1, var2, 8 + var2 * 18, 142));
		}

}

GuiCrafting::~GuiCrafting() {
	m_inventoryCrafting = NULL;
	m_iInventory = NULL;
}

void GuiCrafting::onGuiClosed() {
		GuiContainer::onGuiClosed();

		for(int var1 = 0; var1 < 9; ++var1) {
			ItemStack var2 = m_inventoryCrafting->getStackInSlot(var1);
			if(var2 != NULL) {
				mc->thePlayer->dropPlayerItem(var2);
			}
		}

}

void GuiCrafting::guiCraftingItemsCheck() {
		int* var1 = new int[9];

		for(int var2 = 0; var2 < 3; ++var2) {
			for(int var3 = 0; var3 < 3; ++var3) {
				int var4 = var2 + var3 * 3;
				ItemStack var5 = m_inventoryCrafting->getStackInSlot(var4);
				if(var5 == NULL) {
					var1[var4] = -1;
				} else {
					var1[var4] = var5.itemID;
				}
			}
		}

		m_iInventory->setInventorySlotContents(0, CraftingManager::getInstance().findMatchingRecipe(var1));
}

void GuiCrafting::drawGuiContainerForegroundLayer() {
		fontRenderer->drawString("Crafting", 28, 6, 4210752);
		fontRenderer->drawString("Inventory", 8, ySize - 96 + 2, 4210752);
}

void GuiCrafting::drawGuiContainerBackgroundLayer() {
		int var1 = mc->Textures::load("game:\Content\gui\crafting.png");
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		RenderEngine::bindTexture(var1);
		var1 = (width - xSize) / 2;
		int var2 = (height - ySize) / 2;
		drawTexturedModalRect(var1, var2, 0, 0, xSize, ySize);
}
