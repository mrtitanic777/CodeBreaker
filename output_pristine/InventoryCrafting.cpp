#include "stdafx.h"
// Auto-generated from InventoryCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "InventoryCrafting.h"
#include "Common.h"
#include "GuiContainer.h"
#include "ItemStack.h"
#include <string>

InventoryCrafting::InventoryCrafting()
	: m_stackList(NULL)
	, inventoryWidth(0)
	, m_eventHandler(NULL)
{
}

InventoryCrafting::InventoryCrafting(GuiContainer* var1, int var2, int var3)
	: m_stackList(NULL)
	, inventoryWidth(0)
	, m_eventHandler(NULL)
{
		inventoryWidth = var2 * var3;
		m_stackList = new ItemStack[inventoryWidth];
		m_eventHandler = var1;
}

InventoryCrafting::~InventoryCrafting() {
	delete[] m_stackList;
	m_stackList = NULL;
	m_eventHandler = NULL;
}

int InventoryCrafting::getSizeInventory() {
		return inventoryWidth;
}

ItemStack* InventoryCrafting::getStackInSlot(int var1) {
		return m_stackList[var1];
}

const std::string& InventoryCrafting::getInvName() {
		return "Crafting";
}

ItemStack* InventoryCrafting::decrStackSize(int var1, int var2) {
		if(m_stackList[var1] != NULL) {
			ItemStack var3;
			if(m_stackList[var1].stackSize <= var2) {
				var3 = m_stackList[var1];
				m_stackList[var1] = NULL;
				m_eventHandler->guiCraftingItemsCheck();
				return var3;
			} else {
				var3 = m_stackList[var1].splitStack(var2);
				if(m_stackList[var1].stackSize == 0) {
					m_stackList[var1] = NULL;
				}

				m_eventHandler->guiCraftingItemsCheck();
				return var3;
			}
		} else {
			return NULL;
		}
}

void InventoryCrafting::setInventorySlotContents(int var1, ItemStack* var2) {
		m_stackList[var1] = var2;
		m_eventHandler->guiCraftingItemsCheck();
}

int InventoryCrafting::getInventoryStackLimit() {
		return 64;
}
