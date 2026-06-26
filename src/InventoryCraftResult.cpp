#include "stdafx.h"
// Auto-generated from InventoryCraftResult.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "InventoryCraftResult.h"
#include "Common.h"
#include <string>
#include "ItemStack.h"

InventoryCraftResult::InventoryCraftResult()
	: m_stackResult(new ItemStack[1])
{
}

InventoryCraftResult::~InventoryCraftResult() {
	delete[] m_stackResult;
	m_stackResult = NULL;
}

int InventoryCraftResult::getSizeInventory() {
		return 1;
}

ItemStack InventoryCraftResult::getStackInSlot(int var1) {
		return m_stackResult[var1];
}

const std::string& InventoryCraftResult::getInvName() {
		return "Result";
}

ItemStack InventoryCraftResult::decrStackSize(int var1, int var2) {
		if(m_stackResult[var1] != NULL) {
			ItemStack var3 = m_stackResult[var1];
			m_stackResult[var1] = NULL;
			return var3;
		} else {
			return NULL;
		}
}

void InventoryCraftResult::setInventorySlotContents(int var1, const ItemStack& var2) {
		m_stackResult[var1] = var2;
}

int InventoryCraftResult::getInventoryStackLimit() {
		return 64;
}
