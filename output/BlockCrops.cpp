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
	: BlockFlower(59, 88)
{
		blockIndexInTexture = 88;
		setTickOnLoad(true);
		setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 0.25F, 1.0F);
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
			unsigned char var6 = var1->getBlockMetadata(var2, var3, var4);
			if(var6 < 7) {
				int var11 = var4;
				int var10 = var3;
				int var9 = var2;
				World* var8 = var1;
				float var12 = 1.0F;
				int var13 = var1->getBlockId(var2, var3, var4 - 1);
				int var14 = var1->getBlockId(var2, var3, var4 + 1);
				int var15 = var1->getBlockId(var2 - 1, var3, var4);
				int var16 = var1->getBlockId(var2 + 1, var3, var4);
				int var17 = var1->getBlockId(var2 - 1, var3, var4 - 1);
				int var18 = var1->getBlockId(var2 + 1, var3, var4 - 1);
				int var19 = var1->getBlockId(var2 + 1, var3, var4 + 1);
				int var20 = var1->getBlockId(var2 - 1, var3, var4 + 1);
				bool var23 = var15 == blockID || var16 == blockID;
				bool var22 = var13 == blockID || var14 == blockID;
				bool var7 = var17 == blockID || var18 == blockID || var19 == blockID || var20 == blockID;

				for(var14 = var2 - 1; var14 <= var9 + 1; ++var14) {
					for(var16 = var11 - 1; var16 <= var11 + 1; ++var16) {
						var17 = var8->getBlockId(var14, var10 - 1, var16);
						float var24 = 0.0F;
						if(var17 == Block::s_tilledField->blockID) {
							var24 = 1.0F;
							if(var8->getBlockMetadata(var14, var10 - 1, var16) > 0) {
								var24 = 3.0F;
							}
						}

						if(var14 != var9 || var16 != var11) {
							var24 /= 4.0F;
						}

						var12 += var24;
					}
				}

				if(var7 || var23 && var22) {
					var12 /= 2.0F;
				}

				if(var5->nextInt((int)(100.0F / var12)) == 0) {
					int var21 = var6 + 1;
					var1->setBlockMetadata(var2, var3, var4, var21);
				}
			}
		}

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

		for(int var6 = 0; var6 < 3; ++var6) {
			if(var1->m_random->nextInt(18) <= var5) {
				float var7 = var1->m_random->nextFloat() * 0.7F + 0.15F;
				float var8 = var1->m_random->nextFloat() * 0.7F + 0.15F;
				float var9 = var1->m_random->nextFloat() * 0.7F + 0.15F;
				EntityItem* var10 = new EntityItem(var1, (float)var2 + var7, (float)var3 + var8, (float)var4 + var9, new ItemStack(Item::s_wheat));
				var10->m_delayBeforeCanPickup = 10;
				var1->spawnEntityInWorld(var10);
			}
		}

}

int BlockCrops::idDropped(int var1) {
		DbgPrintF("%s\n", "Get resource: " + var1);
		return var1 == 7 ? Item::s_bread->shiftedIndex : -1;
}

int BlockCrops::quantityDropped(Random* var1) {
		return 1;
}
