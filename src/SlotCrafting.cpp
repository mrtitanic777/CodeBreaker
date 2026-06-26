#include "stdafx.h"
// Auto-generated from SlotCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SlotCrafting.h"
#include "Common.h"
#include "GuiContainer.h"
#include "IInventory.h"
#include "Slot.h"

SlotCrafting::SlotCrafting()
	: m_craftMatrix(NULL)
{
}

SlotCrafting::SlotCrafting(GuiContainer* var1, IInventory* var2, IInventory* var3, int var4, int var5, int var6)
	: Slot(var1, var3, 0, var5, var6)
	, m_craftMatrix(NULL)
{
		m_craftMatrix = var2;
}

SlotCrafting::~SlotCrafting() {
	m_craftMatrix = NULL;
}

bool SlotCrafting::isItemValid() {
		return false;
}

void SlotCrafting::onPickupFromSlot() {
		for(int var1 = 0; var1 < m_craftMatrix->getSizeInventory(); ++var1) {
			if(m_craftMatrix->getStackInSlot(var1) != NULL) {
				m_craftMatrix->decrStackSize(var1, 1);
			}
		}

}
