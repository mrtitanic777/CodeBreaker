#include "stdafx.h"
// Auto-generated from ItemArmor.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemArmor.h"
#include "Common.h"
#include "Item.h"

int* ItemArmor::s_damageReduceAmmountArray = NULL /* PORT: int array init {3, 8, 6, 3} */;
int* ItemArmor::s_maxDamageArray = NULL /* PORT: int array init {11, 16, 15, 13} */;

ItemArmor::ItemArmor()
	: m_armorLevel(0)
	, m_armorType(0)
	, m_damageReduceAmmount(0)
	, m_renderIndex(0)
{
}

ItemArmor::ItemArmor(int var1, int var2, int var3, int var4)
	: Item(var1)
	, m_armorLevel(0)
	, m_armorType(0)
	, m_damageReduceAmmount(0)
	, m_renderIndex(0)
{
		m_armorLevel = var2;
		m_armorType = var4;
		m_renderIndex = var3;
		m_damageReduceAmmount = s_damageReduceAmmountArray[var4];
		maxDamage = s_maxDamageArray[var4] * 3 << var2;
		maxStackSize = 1;
}

ItemArmor::~ItemArmor() {
	// No dynamic resources
}
