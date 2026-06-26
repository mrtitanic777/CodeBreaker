#include "stdafx.h"
// Auto-generated from ItemFlintAndSteel.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemFlintAndSteel.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "Block.h"
#include "Item.h"
#include "BlockFire.h"
#include "Random.h"

ItemFlintAndSteel::ItemFlintAndSteel()
{
}

ItemFlintAndSteel::ItemFlintAndSteel(int var1)
	: Item(var1)
{
		maxStackSize = 1;
		maxDamage = 64;
}

ItemFlintAndSteel::~ItemFlintAndSteel() {
	// No dynamic resources
}

bool ItemFlintAndSteel::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7) {
		if(var7 == 0) {
			--var5;
		}

		if(var7 == 1) {
			++var5;
		}

		if(var7 == 2) {
			--var6;
		}

		if(var7 == 3) {
			++var6;
		}

		if(var7 == 4) {
			--var4;
		}

		if(var7 == 5) {
			++var4;
		}

		int var8 = var3->getBlockId(var4, var5, var6);
		if(var8 == 0) {
			var3->playSoundEffect((double)var4 + 0.5f, (double)var5 + 0.5f, (double)var6 + 0.5f, "fire.ignite", 1.0F, Item::s_itemRand->nextFloat() * 0.4F + 0.8F);
			var3->setBlockWithNotify(var4, var5, var6, Block::s_fire->blockID);
		}

		var1->damageItem(1);
		return true;
}
