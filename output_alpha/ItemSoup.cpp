#include "stdafx.h"
// Auto-generated from ItemSoup.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSoup.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "Item.h"
#include "ItemFood.h"

ItemSoup::ItemSoup()
{
}

ItemSoup::ItemSoup(int var1, int var2)
	: ItemFood(var1, var2)
{
}

ItemSoup::~ItemSoup() {
	// No dynamic resources
}

ItemStack* ItemSoup::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3) {
		ItemFood::onItemRightClick(var1, var2, var3);
		return new ItemStack(Item::s_bowlEmpty);
}
