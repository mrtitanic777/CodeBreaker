#include "stdafx.h"
// Auto-generated from Slot.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Slot.h"
#include "Common.h"
#include "IInventory.h"
#include "ItemStack.h"

Slot::Slot()
	: m_slotIndex(0)
	, m_inventory(NULL)
{
}

Slot::Slot(IInventory* var1, int var2)
	: m_slotIndex(0)
	, m_inventory(NULL)
{
		m_inventory = var1;
		m_slotIndex = var2;
}

Slot::~Slot() {
	m_inventory = NULL;
}

void Slot::onPickupFromSlot() {
		onSlotChanged();
}

bool Slot::isItemValid(ItemStack* var1) {
		return true;
}

ItemStack* Slot::getStack() {
		return m_inventory->getStackInSlot(m_slotIndex);
}

void Slot::putStack(ItemStack* var1) {
		m_inventory->setInventorySlotContents(m_slotIndex, var1);
		onSlotChanged();
}

int Slot::func_775_c() {
		return -1;
}

void Slot::onSlotChanged() {
		m_inventory->onInventoryChanged();
}

int Slot::getSlotStackLimit() {
		return m_inventory->getInventoryStackLimit();
}
