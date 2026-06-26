#include "stdafx.h"
// Auto-generated from Slot.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Slot.h"
#include "Common.h"
#include "GuiContainer.h"
#include "IInventory.h"
#include "ItemStack.h"

Slot::Slot()
	: m_slotIndex(0)
	, m_xPos(0)
	, m_yPos(0)
	, m_inventory(NULL)
	, m_guiHandler(NULL)
{
}

Slot::Slot(GuiContainer* var1, IInventory* var2, int var3, int var4, int var5)
	: m_slotIndex(0)
	, m_xPos(0)
	, m_yPos(0)
	, m_inventory(NULL)
	, m_guiHandler(NULL)
{
		m_guiHandler = var1;
		m_inventory = var2;
		m_slotIndex = var3;
		m_xPos = var4;
		m_yPos = var5;
}

Slot::~Slot() {
	m_inventory = NULL;
	m_guiHandler = NULL;
}

bool Slot::isAtCursorPos(int var1, int var2) {
		int var3 = (m_guiHandler->width - m_guiHandler->m_xSize) / 2;
		int var4 = (m_guiHandler->height - m_guiHandler->m_ySize) / 2;
		var1 -= var3;
		var2 -= var4;
		return var1 >= m_xPos - 1 && var1 < m_xPos + 16 + 1 && var2 >= m_yPos - 1 && var2 < m_yPos + 16 + 1;
}

void Slot::onPickupFromSlot() {
	// TODO: implement
}

bool Slot::isItemValid() {
		return true;
}

void Slot::putStack(ItemStack* var1) {
		m_inventory->setInventorySlotContents(m_slotIndex, var1);
}
