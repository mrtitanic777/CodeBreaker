#include "stdafx.h"
// Auto-generated from CraftingInventoryPlayerCB.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CraftingInventoryPlayerCB.h"
#include "Common.h"
#include "EntityPlayerSP.h"
#include "EntityPlayer.h"
#include "IInventory.h"
#include "InventoryCrafting.h"
#include "ItemStack.h"
#include "CraftingManager.h"
#include "CraftingInventoryCB.h"
#include "InventoryCraftResult.h"

CraftingInventoryPlayerCB::CraftingInventoryPlayerCB()
	: m_craftMatrix(NULL)
	, m_craftResult(new InventoryCraftResult())
{
}

CraftingInventoryPlayerCB::CraftingInventoryPlayerCB(ItemStack** var1)
	: m_craftMatrix(NULL)
	, m_craftResult(new InventoryCraftResult())
{
		m_craftMatrix = new InventoryCrafting(this, var1);
		onCraftMatrixChanged(m_craftMatrix);
}

CraftingInventoryPlayerCB::~CraftingInventoryPlayerCB() {
	m_craftMatrix = NULL;
	m_craftResult = NULL;
}

void CraftingInventoryPlayerCB::onCraftMatrixChanged(IInventory* var1) {
		int* var2 = new int[9];

		for(int var3 = 0; var3 < 3; ++var3) {
			for(int var4 = 0; var4 < 3; ++var4) {
				int var5 = -1;
				if(var3 < 2 && var4 < 2) {
					ItemStack* var6 = m_craftMatrix->getStackInSlot(var3 + var4 * 2);
					if(var6 != NULL) {
						var5 = var6->itemID;
					}
				}

				var2[var3 + var4 * 3] = var5;
			}
		}

		m_craftResult->setInventorySlotContents(0, CraftingManager::getInstance()->craft(var2));
}

void CraftingInventoryPlayerCB::onCraftGuiClosed(EntityPlayer* var1) {
		CraftingInventoryCB::onCraftGuiClosed(var1);

		for(int var2 = 0; var2 < 9; ++var2) {
			ItemStack* var3 = m_craftMatrix->getStackInSlot(var2);
			if(var3 != NULL) {
				var1->dropPlayerItem(var3);
			}
		}

}
