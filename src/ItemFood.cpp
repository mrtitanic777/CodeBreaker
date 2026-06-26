#include "stdafx.h"
// Auto-generated from ItemFood.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemFood.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "Item.h"

ItemFood::ItemFood()
	: m_healAmount(0)
{
}

ItemFood::ItemFood(int var1, int var2)
	: Item(var1)
	, m_healAmount(0)
{
		m_healAmount = var2;
		maxStackSize = 1;
}

ItemFood::~ItemFood() {
	// No dynamic resources
}

ItemStack ItemFood::onItemRightClick(const ItemStack& var1, World* var2, EntityPlayer* var3) {
		--var1.stackSize;
		var3->heal(m_healAmount);
		return var1;
}
