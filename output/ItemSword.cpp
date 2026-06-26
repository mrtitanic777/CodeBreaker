#include "stdafx.h"
// Auto-generated from ItemSword.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSword.h"
#include "Common.h"
#include "Block.h"
#include "ItemStack.h"
#include "Item.h"

ItemSword::ItemSword()
	: m_weaponDamage(0)
{
}

ItemSword::ItemSword(int var1, int var2)
	: Item(var1)
	, m_weaponDamage(0)
{
		maxStackSize = 1;
		maxDamage = 32 << var2;
		m_weaponDamage = 4 + (var2 << 1);
}

ItemSword::~ItemSword() {
	// No dynamic resources
}

float ItemSword::getStrVsBlock(Block* var1) {
		return 1.5F;
}

void ItemSword::hitEntity(ItemStack* var1) {
		var1->damageItem(1);
}

void ItemSword::onBlockDestroyed(ItemStack* var1) {
		var1->damageItem(2);
}

int ItemSword::getDamageVsEntity() {
		return m_weaponDamage;
}
