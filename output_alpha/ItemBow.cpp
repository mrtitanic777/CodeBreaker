#include "stdafx.h"
// Auto-generated from ItemBow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemBow.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "EntityArrow.h"
#include "Item.h"
#include "InventoryPlayer.h"
#include "Random.h"

ItemBow::ItemBow()
{
}

ItemBow::ItemBow(int var1)
	: Item(var1)
{
		maxStackSize = 1;
}

ItemBow::~ItemBow() {
	// No dynamic resources
}

ItemStack* ItemBow::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3) {
		if(var3->m_inventory->consumeInventoryItem(Item::s_arrow->shiftedIndex)) {
			var2->playSoundAtEntity(var3, "random.bow", 1.0F, 1.0F / (Item::s_itemRand->nextFloat() * 0.4F + 0.8F));
			if(!var2->m_multiplayerWorld) {
				var2->entityJoinedWorld(new EntityArrow(var2, var3));
			}
		}

		return var1;
}
