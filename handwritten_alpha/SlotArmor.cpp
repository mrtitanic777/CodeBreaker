#include "stdafx.h"
// Auto-generated from SlotArmor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SlotArmor.h"
#include "Common.h"
#include "Item.h"
#include "GuiContainer.h"
#include "GuiInventory.h"
#include "IInventory.h"
#include "ItemStack.h"
#include "ItemArmor.h"
#include "Block.h"

SlotArmor::SlotArmor()
	: m_field_1124_c(0)
	, m_field_1123_d(NULL)
{
}

SlotArmor::SlotArmor(GuiInventory* var1, GuiContainer* var2, IInventory* var3, int var4, int var5, int var6, int var7)
	: SlotInventory(var2, var3, var4, var5, var6)
	, m_field_1124_c(0)
	, m_field_1123_d(NULL)
{
		m_field_1123_d = var1;
		m_field_1124_c = var7;
}

SlotArmor::~SlotArmor() {
	m_field_1123_d = NULL;
}

int SlotArmor::getSlotStackLimit() {
		return 1;
}

bool SlotArmor::isItemValid(ItemStack* var1) {
		// PORT: Java `var1.getItem() instanceof ItemArmor` -> dynamic_cast (Item is polymorphic)
		ItemArmor* var2 = dynamic_cast<ItemArmor*>(var1->getItem());
		if(var2 != NULL) {
			return var2->m_armorType == m_field_1124_c;
		} else {
			DbgPrintF("%s\n", (toStr(var1->getItem()->shiftedIndex) + ", " + toStr(m_field_1124_c)).c_str());
			return var1->getItem()->shiftedIndex == Block::s_pumpkin->blockID ? m_field_1124_c == 0 : false;
		}
}

int SlotArmor::func_775_c() {
		return 15 + m_field_1124_c * 16;
}
