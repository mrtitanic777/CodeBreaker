#include "stdafx.h"
// Auto-generated from CraftingInventoryCB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CraftingInventoryCB.h"
#include "Common.h"
#include "EntityPlayerSP.h"
#include "EntityPlayer.h"
#include "IInventory.h"
#include "InventoryPlayer.h"

CraftingInventoryCB::CraftingInventoryCB()
{
}

CraftingInventoryCB::~CraftingInventoryCB() {
	// No dynamic resources
}

void CraftingInventoryCB::onCraftGuiClosed(EntityPlayer* var1) {
		InventoryPlayer* var2 = var1->m_inventory;
		if(var2->m_draggingItemStack != NULL) {
			var1->dropPlayerItem(var2->m_draggingItemStack);
		}

}

void CraftingInventoryCB::onCraftMatrixChanged(IInventory* var1) {
	// TODO: implement
}
