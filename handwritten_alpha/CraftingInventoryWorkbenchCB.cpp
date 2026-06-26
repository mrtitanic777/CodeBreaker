#include "stdafx.h"
// Auto-generated from CraftingInventoryWorkbenchCB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CraftingInventoryWorkbenchCB.h"
#include "Common.h"
#include "EntityPlayerSP.h"
#include "EntityPlayer.h"
#include "IInventory.h"
#include "InventoryCrafting.h"
#include "CraftingManager.h"
#include "ItemStack.h"
#include "CraftingInventoryCB.h"
#include "InventoryCraftResult.h"

CraftingInventoryWorkbenchCB::CraftingInventoryWorkbenchCB()
	: m_craftMatrix(new InventoryCrafting(this, 3, 3))
	, m_craftResult(new InventoryCraftResult())
{
}

CraftingInventoryWorkbenchCB::~CraftingInventoryWorkbenchCB() {
	m_craftMatrix = NULL;
	m_craftResult = NULL;
}

void CraftingInventoryWorkbenchCB::onCraftMatrixChanged(IInventory* var1) {
		int* var2 = new int[9];

		for(int var3 = 0; var3 < 3; ++var3) {
			for(int var4 = 0; var4 < 3; ++var4) {
				int var5 = var3 + var4 * 3;
				ItemStack* var6 = m_craftMatrix->getStackInSlot(var5);
				if(var6 == NULL) {
					var2[var5] = -1;
				} else {
					var2[var5] = var6->itemID;
				}
			}
		}

		m_craftResult->setInventorySlotContents(0, CraftingManager::getInstance()->craft(var2));
}

void CraftingInventoryWorkbenchCB::onCraftGuiClosed(EntityPlayer* var1) {
		CraftingInventoryCB::onCraftGuiClosed(var1);

		for(int var2 = 0; var2 < 9; ++var2) {
			ItemStack* var3 = m_craftMatrix->getStackInSlot(var2);
			if(var3 != NULL) {
				var1->dropPlayerItem(var3);
			}
		}

}
