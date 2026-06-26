#include "stdafx.h"
// Auto-generated from InventoryLargeChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "InventoryLargeChest.h"
#include "Common.h"
#include "ItemStack.h"
#include <string>
#include "IInventory.h"

InventoryLargeChest::InventoryLargeChest()
	: m_upperChest(NULL)
	, m_lowerChest(NULL)
{
}

InventoryLargeChest::InventoryLargeChest(const std::string& var1, IInventory* var2, IInventory* var3)
	: m_upperChest(NULL)
	, m_lowerChest(NULL)
{
		m_name = var1;
		m_upperChest = var2;
		m_lowerChest = var3;
}

InventoryLargeChest::~InventoryLargeChest() {
	m_upperChest = NULL;
	m_lowerChest = NULL;
}

int InventoryLargeChest::getSizeInventory() {
		return m_upperChest->getSizeInventory() + m_lowerChest->getSizeInventory();
}

const std::string& InventoryLargeChest::getInvName() {
		return m_name;
}

ItemStack* InventoryLargeChest::getStackInSlot(int var1) {
		return var1 >= m_upperChest->getSizeInventory() ? m_lowerChest->getStackInSlot(var1 - m_upperChest->getSizeInventory()) : m_upperChest->getStackInSlot(var1);
}

ItemStack* InventoryLargeChest::decrStackSize(int var1, int var2) {
		return var1 >= m_upperChest->getSizeInventory() ? m_lowerChest->decrStackSize(var1 - m_upperChest->getSizeInventory(), var2) : m_upperChest->decrStackSize(var1, var2);
}

void InventoryLargeChest::setInventorySlotContents(int var1, ItemStack* var2) {
		if(var1 >= m_upperChest->getSizeInventory()) {
			m_lowerChest->setInventorySlotContents(var1 - m_upperChest->getSizeInventory(), var2);
		} else {
			m_upperChest->setInventorySlotContents(var1, var2);
		}
}

int InventoryLargeChest::getInventoryStackLimit() {
		return m_upperChest->getInventoryStackLimit();
}
