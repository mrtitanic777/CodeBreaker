#include "stdafx.h"
// Auto-generated from ItemFlintAndSteel.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemFlintAndSteel.h"
#include "Common.h"
#include "ItemStack.h"
#include "World.h"
#include "Block.h"
#include "Item.h"

ItemFlintAndSteel::ItemFlintAndSteel()
{
}

ItemFlintAndSteel::ItemFlintAndSteel(int var1)
	: Item(3)
{
		maxStackSize = 1;
		maxDamage = 64;
}

ItemFlintAndSteel::~ItemFlintAndSteel() {
	// No dynamic resources
}

bool ItemFlintAndSteel::onItemUse(ItemStack* var1, World* var2, int var3, int var4, int var5, int var6) {
		if(var6 == 0) {
			--var4;
		}

		if(var6 == 1) {
			++var4;
		}

		if(var6 == 2) {
			--var5;
		}

		if(var6 == 3) {
			++var5;
		}

		if(var6 == 4) {
			--var3;
		}

		if(var6 == 5) {
			++var3;
		}

		if(var3 > 0 && var4 > 0 && var5 > 0 && var3 < var2->width - 1 && var4 < var2->height - 1 && var5 < var2_count /* PORT: set this from allocation */ - 1) {
			var6 = var2->getBlockId(var3, var4, var5);
			if(var6 == 0) {
				var2->playSoundAtPlayer((float)var3 + 0.5F, (float)var4 + 0.5F, (float)var5 + 0.5F, "fire.ignite", 1.0F, rand->nextFloat() * 0.4F + 0.8F);
				var2->setBlockWithNotify(var3, var4, var5, Block::s_fire->blockID);
			}

			var1->damageItem(1);
			return true;
		} else {
			return false;
		}
}
