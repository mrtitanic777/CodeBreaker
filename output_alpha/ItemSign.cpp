#include "stdafx.h"
// Auto-generated from ItemSign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSign.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include "Item.h"
#include "Block.h"
#include "Material.h"
#include "TileEntitySign.h"

ItemSign::ItemSign()
{
}

ItemSign::ItemSign(int var1)
	: Item(var1)
{
		maxDamage = 64;
		maxStackSize = 1;
}

ItemSign::~ItemSign() {
	// No dynamic resources
}

bool ItemSign::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7) {
		if(var7 == 0) {
			return false;
		} else if(!var3->getBlockMaterial(var4, var5, var6)->func_878_a()) {
			return false;
		} else {
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

			if(!Block::s_signPost->canPlaceBlockAt(var3, var4, var5, var6)) {
				return false;
			} else {
				if(var7 == 1) {
					var3->setBlockAndMetadataWithNotify(var4, var5, var6, Block::s_signPost->blockID, (int)floor((double)((var2->rotationYaw + 180.0F) * 16.0F / 360.0F) + 0.5f) & 15);
				} else {
					var3->setBlockAndMetadataWithNotify(var4, var5, var6, Block::s_signWall->blockID, var7);
				}

				--var1->stackSize;
				TileEntitySign* var8 = (TileEntitySign*)var3->getBlockTileEntity(var4, var5, var6);
				if(var8 != NULL) {
					var2->displayGUIEditSign(var8);
				}

				return true;
			}
		}
}
