#include "stdafx.h"
// Auto-generated from ItemSword.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSword.h"
#include "Common.h"
#include "Block.h"
#include "Entity.h"
#include "EntityLiving.h"
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
		if(var2 == 3) {
			maxDamage *= 4;
		}

		m_weaponDamage = 4 + var2 * 2;
}

ItemSword::~ItemSword() {
	// No dynamic resources
}

float ItemSword::getStrVsBlock(ItemStack* var1, Block* var2) {
		return 1.5F;
}

void ItemSword::hitEntity(ItemStack* var1, EntityLiving* var2) {
		var1->damageItem(1);
}

void ItemSword::hitBlock(ItemStack* var1, int var2, int var3, int var4, int var5) {
		var1->damageItem(2);
}

int ItemSword::getDamageVsEntity(Entity* var1) {
		return m_weaponDamage;
}

bool ItemSword::isFull3D() {
		return true;
}
