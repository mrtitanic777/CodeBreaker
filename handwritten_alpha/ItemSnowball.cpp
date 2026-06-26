#include "stdafx.h"
// Auto-generated from ItemSnowball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSnowball.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "Item.h"
#include "EntitySnowball.h"
#include "Random.h"

ItemSnowball::ItemSnowball()
{
}

ItemSnowball::ItemSnowball(int var1)
	: Item(var1)
{
		maxStackSize = 16;
}

ItemSnowball::~ItemSnowball() {
	// No dynamic resources
}

ItemStack* ItemSnowball::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3) {
		--var1->stackSize;
		var2->playSoundAtEntity(var3, "random.bow", 0.5F, 0.4F / (Item::s_itemRand->nextFloat() * 0.4F + 0.8F));
		if(!var2->m_multiplayerWorld) {
			var2->entityJoinedWorld(new EntitySnowball(var2, var3));
		}

		return var1;
}
