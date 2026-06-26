#include "stdafx.h"
// Auto-generated from ItemFishingRod.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemFishingRod.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "Item.h"
#include "EntityFish.h"
#include "Random.h"

ItemFishingRod::ItemFishingRod()
{
}

ItemFishingRod::ItemFishingRod(int var1)
	: Item(var1)
{
		maxDamage = 64;
}

ItemFishingRod::~ItemFishingRod() {
	// No dynamic resources
}

bool ItemFishingRod::isFull3D() {
		return true;
}

bool ItemFishingRod::shouldRotateAroundWhenRendering() {
		return true;
}

ItemStack* ItemFishingRod::onItemRightClick(ItemStack* var1, World* var2, EntityPlayer* var3) {
		if(var3->m_fishEntity != NULL) {
			int var4 = var3->m_fishEntity->func_4043_i();
			var1->damageItem(var4);
			var3->func_457_w();
		} else {
			var2->playSoundAtEntity(var3, "random.bow", 0.5F, 0.4F / (Item::s_itemRand->nextFloat() * 0.4F + 0.8F));
			if(!var2->m_multiplayerWorld) {
				var2->entityJoinedWorld(new EntityFish(var2, var3));
			}

			var3->func_457_w();
		}

		return var1;
}
