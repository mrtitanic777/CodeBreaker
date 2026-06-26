#include "stdafx.h"
// Auto-generated from BlockCrops.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockCrops.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "EntityItem.h"
#include "Item.h"
#include "ItemStack.h"
#include "BlockFlower.h"

BlockCrops::BlockCrops()
{
}

BlockCrops::BlockCrops(int var1, int var2)
	: BlockFlower(var1, var2)
{
		blockIndexInTexture = var2;
		setTickOnLoad(true);
		float var3 = 0.5F;
		setBlockBounds(0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, 0.25F, 0.5F + var3);
}

BlockCrops::~BlockCrops() {
	// No dynamic resources
}

bool BlockCrops::canThisPlantGrowOnThisBlockID(int var1) {
		return var1 == Block::s_tilledField->blockID;
}

void BlockCrops::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		BlockFlower::updateTick(var1, var2, var3, var4, var5);
		if(var1->getBlockLightValue(var2, var3 + 1, var4) >= 9) {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			if(var6 < 7) {
				float var7 = getGrowthRate(var1, var2, var3, var4);
				if(var5->nextInt((int)(100.0F / var7)) == 0) {
					++var6;
					var1->setBlockMetadataWithNotify(var2, var3, var4, var6);
				}
			}
		}

}

float BlockCrops::getGrowthRate(World* var1, int var2, int var3, int var4) {
		float var5 = 1.0F;
		int var6 = var1->getBlockId(var2, var3, var4 - 1);
		int var7 = var1->getBlockId(var2, var3, var4 + 1);
		int var8 = var1->getBlockId(var2 - 1, var3, var4);
		int var9 = var1->getBlockId(var2 + 1, var3, var4);
		int var10 = var1->getBlockId(var2 - 1, var3, var4 - 1);
		int var11 = var1->getBlockId(var2 + 1, var3, var4 - 1);
		int var12 = var1->getBlockId(var2 + 1, var3, var4 + 1);
		int var13 = var1->getBlockId(var2 - 1, var3, var4 + 1);
		bool var14 = var8 == blockID || var9 == blockID;
		bool var15 = var6 == blockID || var7 == blockID;
		bool var16 = var10 == blockID || var11 == blockID || var12 == blockID || var13 == blockID;

		for(int var17 = var2 - 1; var17 <= var2 + 1; ++var17) {
			for(int var18 = var4 - 1; var18 <= var4 + 1; ++var18) {
				int var19 = var1->getBlockId(var17, var3 - 1, var18);
				float var20 = 0.0F;
				if(var19 == Block::s_tilledField->blockID) {
					var20 = 1.0F;
					if(var1->getBlockMetadata(var17, var3 - 1, var18) > 0) {
						var20 = 3.0F;
					}
				}

				if(var17 != var2 || var18 != var4) {
					var20 /= 4.0F;
				}

				var5 += var20;
			}
		}

		if(var16 || var14 && var15) {
			var5 /= 2.0F;
		}

		return var5;
}

int BlockCrops::getBlockTextureFromSideAndMetadata(int var1, int var2) {
		if(var2 < 0) {
			var2 = 7;
		}

		return blockIndexInTexture + var2;
}

int BlockCrops::getRenderType() {
		return 6;
}

void BlockCrops::onBlockDestroyedByPlayer(World* var1, int var2, int var3, int var4, int var5) {
		BlockFlower::onBlockDestroyedByPlayer(var1, var2, var3, var4, var5);
		if(!var1->m_multiplayerWorld) {
			for(int var6 = 0; var6 < 3; ++var6) {
				if(var1->rand->nextInt(15) <= var5) {
					float var7 = 0.7F;
					float var8 = var1->rand->nextFloat() * var7 + (1.0F - var7) * 0.5F;
					float var9 = var1->rand->nextFloat() * var7 + (1.0F - var7) * 0.5F;
					float var10 = var1->rand->nextFloat() * var7 + (1.0F - var7) * 0.5F;
					EntityItem* var11 = new EntityItem(var1, (double)((float)var2 + var8), (double)((float)var3 + var9), (double)((float)var4 + var10), new ItemStack(Item::s_seeds));
					var11->m_field_805_c = 10;
					var1->entityJoinedWorld(var11);
				}
			}
		}

}

int BlockCrops::idDropped(int var1, Random* var2) {
		return var1 == 7 ? Item::s_wheat->shiftedIndex : -1;
}

int BlockCrops::quantityDropped(Random* var1) {
		return 1;
}
