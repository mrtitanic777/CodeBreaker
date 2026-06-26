#include "stdafx.h"
// Auto-generated from ItemRedstone.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemRedstone.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "Item.h"
#include "Block.h"

ItemRedstone::ItemRedstone()
{
}

ItemRedstone::ItemRedstone(int var1)
	: Item(var1)
{
}

ItemRedstone::~ItemRedstone() {
	// No dynamic resources
}

bool ItemRedstone::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7) {
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

		if(var3->getBlockId(var4, var5, var6) != 0) {
			return false;
		} else {
			if(Block::s_redstoneWire->canPlaceBlockAt(var3, var4, var5, var6)) {
				--var1->stackSize;
				var3->setBlockWithNotify(var4, var5, var6, Block::s_redstoneWire->blockID);
			}

			return true;
		}
}
