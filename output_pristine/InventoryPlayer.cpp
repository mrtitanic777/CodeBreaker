#include "stdafx.h"
// Auto-generated from InventoryPlayer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "InventoryPlayer.h"
#include "Common.h"
#include "ItemStack.h"
#include <string>
#include "Player.h"

InventoryPlayer::InventoryPlayer()
	: m_mainInventory(new ItemStack[40])
	, m_currentItem(0)
{
}

InventoryPlayer::~InventoryPlayer() {
	delete[] m_mainInventory;
	m_mainInventory = NULL;
}

ItemStack* InventoryPlayer::getCurrentItem() {
		return m_mainInventory[m_currentItem];
}

int InventoryPlayer::getInventorySlotContainItem(int var1) {
		for(int var2 = 0; var2 < m_mainInventory_count /* PORT: set this from allocation */; ++var2) {
			if(m_mainInventory[var2] != NULL && m_mainInventory[var2].itemID == var1) {
				return var2;
			}
		}

		return -1;
}

int InventoryPlayer::storeItemStack() {
		for(int var1 = 0; var1 < m_mainInventory_count /* PORT: set this from allocation */; ++var1) {
			if(m_mainInventory[var1] == NULL) {
				return var1;
			}
		}

		return -1;
}

void InventoryPlayer::getFirstEmptyStack(int var1) {
		var1 = getInventorySlotContainItem(var1);
		if(var1 >= 0 && var1 < 9) {
			m_currentItem = var1;
		}
}

bool InventoryPlayer::consumeInventoryItem(int var1) {
		var1 = getInventorySlotContainItem(var1);
		if(var1 < 0) {
			return false;
		} else {
			if(--m_mainInventory[var1].stackSize <= 0) {
				m_mainInventory[var1] = NULL;
			}

			return true;
		}
}

bool InventoryPlayer::storePartialItemStack(ItemStack* var1) {
		if(var1->itemDamage == 0) {
			int var4 = var1->stackSize;
			int var3 = var1->itemID;
			int var6 = var3;
			InventoryPlayer var5 = this;
			int var7 = 0;

			int var10001;
			ItemStack var8;
			while(true) {
				if(var7 >= var5.m_mainInventory /* PORT: array length - use tracked size */) {
					var10001 = -1;
					break;
				}

				if(var5.m_mainInventory[var7] != NULL && var5.m_mainInventory[var7].itemID == var6) {
					var8 = var5.m_mainInventory[var7];
					if(var5.m_mainInventory[var7].stackSize < var8.getItem().getItemStackLimit() && var5.m_mainInventory[var7].stackSize < 64) {
						var10001 = var7;
						break;
					}
				}

				++var7;
			}

			int var9 = var10001;
			if(var9 < 0) {
				var9 = storeItemStack();
			}

			if(var9 < 0) {
				var10001 = var4;
			} else {
				if(m_mainInventory[var9] == NULL) {
					m_mainInventory[var9] = ItemStack(var3, 0);
				}

				var3 = var4;
				var8 = m_mainInventory[var9];
				if(var4 > var8.getItem().getItemStackLimit() - m_mainInventory[var9].stackSize) {
					var8 = m_mainInventory[var9];
					var3 = var8.getItem().getItemStackLimit() - m_mainInventory[var9].stackSize;
				}

				if(var3 > 64 - m_mainInventory[var9].stackSize) {
					var3 = 64 - m_mainInventory[var9].stackSize;
				}

				if(var3 == 0) {
					var10001 = var4;
				} else {
					var4 -= var3;
					m_mainInventory[var9].stackSize += var3;
					m_mainInventory[var9].m_animationsToGo = 5;
					var10001 = var4;
				}
			}

			var1->stackSize = var10001;
			if(var1->stackSize == 0) {
				return true;
			}
		}

		int var2 = storeItemStack();
		if(var2 >= 0) {
			m_mainInventory[var2] = var1;
			m_mainInventory[var2].m_animationsToGo = 5;
			return true;
		} else {
			return false;
		}
}

ItemStack* InventoryPlayer::decrStackSize(int var1, int var2) {
		if(m_mainInventory[var1] != NULL) {
			ItemStack var3;
			if(m_mainInventory[var1].stackSize <= var2) {
				var3 = m_mainInventory[var1];
				m_mainInventory[var1] = NULL;
				return var3;
			} else {
				var3 = m_mainInventory[var1].splitStack(var2);
				if(m_mainInventory[var1].stackSize == 0) {
					m_mainInventory[var1] = NULL;
				}

				return var3;
			}
		} else {
			return NULL;
		}
}

void InventoryPlayer::setInventorySlotContents(int var1, ItemStack* var2) {
		m_mainInventory[var1] = var2;
}

int InventoryPlayer::getSizeInventory() {
		return m_mainInventory /* PORT: array length - use tracked size */;
}

ItemStack* InventoryPlayer::getStackInSlot(int var1) {
		return m_mainInventory[var1];
}

const std::string& InventoryPlayer::getInvName() {
		return "Inventory";
}

int InventoryPlayer::getInventoryStackLimit() {
		return 64;
}
