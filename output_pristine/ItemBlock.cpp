#include "stdafx.h"
// Auto-generated from ItemBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemBlock.h"
#include "Common.h"
#include "ItemStack.h"
#include "World.h"
#include <string>
#include "AxisAlignedBB.h"
#include "Block.h"
#include "Item.h"
#include "StepSound.h"

ItemBlock::ItemBlock()
	: blockID(0)
{
}

ItemBlock::ItemBlock(int var1)
	: Item(var1)
	, blockID(0)
{
		blockID = var1 + 256;
		setIconIndex(Block::blocksList[var1 + 256].getBlockTextureFromSide(2));
}

ItemBlock::~ItemBlock() {
	// No dynamic resources
}

bool ItemBlock::onItemUse(ItemStack* var1, World* var2, int var3, int var4, int var5, int var6) {
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

		if(var1->stackSize == 0) {
			return false;
		} else if(var3 > 0 && var4 > 0 && var5 > 0 && var3 < var2->width - 1 && var4 < var2->height - 1 && var5 < var2_count /* PORT: set this from allocation */ - 1) {
			int var7 = var2->getBlockId(var3, var4, var5);
			Block var10 = Block::blocksList[var7];
			if(blockID > 0 && var10 == NULL || var10 == Block::s_waterMoving || var10 == Block::s_waterStill || var10 == Block::s_lavaMoving || var10 == Block::s_lavaStill || var10 == Block::s_fire) {
				var10 = Block::blocksList[blockID];
				AxisAlignedBB var8 = var10.getCollisionBoundingBoxFromPool(var3, var4, var5);
				if(var2->checkIfAABBIsClear(var8) && var10.canPlaceBlockAt(var2, var3, var4, var5) && var2->setBlockWithNotify(var3, var4, var5, blockID)) {
					Block::blocksList[blockID].onBlockPlaced(var2, var3, var4, var5, var6);
					float var10001 = (float)var3 + 0.5F;
					float var10002 = (float)var4 + 0.5F;
					float var10003 = (float)var5 + 0.5F;
					std::string var10004 = var10.stepSound->stepSoundDir2();
					StepSound var9 = var10.stepSound;
					float var10005 = (var9.m_soundVolume + 1.0F) / 2.0F;
					var9 = var10.stepSound;
					var2->playSoundAtPlayer(var10001, var10002, var10003, var10004, var10005, var9.m_soundPitch * 0.8F);
					--var1->stackSize;
				}
			}

			return true;
		} else {
			return false;
		}
}

bool ItemBlock::onPlaced(World* var1, float var2, float var3, float var4) {
		return Block::blocksList[blockID].onBlockPlaced(var1, var2, var3, var4);
}
