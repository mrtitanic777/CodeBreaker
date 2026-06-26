#include "stdafx.h"
// Auto-generated from BlockFarmland.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFarmland.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockFarmland::BlockFarmland()
{
}

BlockFarmland::BlockFarmland(int var1)
	: Block(60, Material::s_ground)
{
		blockIndexInTexture = 87;
		setTickOnLoad(true);
		setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 15.0F / 16.0F, 1.0F);
		setLightOpacity(255);
}

BlockFarmland::~BlockFarmland() {
	// No dynamic resources
}

AxisAlignedBB BlockFarmland::getCollisionBoundingBoxFromPool(int var1, int var2, int var3) {
		return AxisAlignedBB((float)var1, (float)var2, (float)var3, (float)(var1 + 1), (float)(var2 + 1), (float)(var3 + 1));
}

bool BlockFarmland::isOpaqueCube() {
		return false;
}

bool BlockFarmland::renderAsNormalBlock() {
		return false;
}

int BlockFarmland::getBlockTextureFromSideAndMetadata(int var1, int var2) {
		return var1 == 1 && var2 > 0 ? blockIndexInTexture - 1 : (var1 == 1 ? blockIndexInTexture : 2);
}

void BlockFarmland::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var5->nextInt(5) == 0) {
			int var8 = var4;
			int var7 = var3;
			int var6 = var2;
			World var12 = var1;
			int var9 = var2 - 4;

			int var10;
			int var11;
			bool var10000;
			label69:
			while(true) {
				if(var9 > var6 + 4) {
					var10000 = false;
					break;
				}

				for(var10 = var7; var10 <= var7 + 1; ++var10) {
					for(var11 = var8 - 4; var11 <= var8 + 4; ++var11) {
						if(var12.getBlockMaterial(var9, var10, var11) == Material::s_water) {
							var10000 = true;
							goto label69;
						}
					}
				}

				++var9;
			}

			if(var10000) {
				var1->setBlockMetadata(var2, var3, var4, 7);
				return;
			}

			unsigned char var13 = var1->getBlockMetadata(var2, var3, var4);
			if(var13 > 0) {
				var1->setBlockMetadata(var2, var3, var4, var13 - 1);
				return;
			}

			var8 = var4;
			var7 = var3;
			var6 = var2;
			var12 = var1;
			var10 = var2;

			label49:
			while(true) {
				if(var10 > var6) {
					var10000 = false;
					break;
				}

				for(var11 = var8; var11 <= var8; ++var11) {
					if(var12.getBlockId(var10, var7 + 1, var11) == Block::s_crops->blockID) {
						var10000 = true;
						goto label49;
					}
				}

				++var10;
			}

			if(!var10000) {
				var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
			}
		}

}

void BlockFarmland::onEntityWalking(World* var1, int var2, int var3, int var4) {
		if(var1->m_random->nextInt(4) == 0) {
			var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
		}

}

void BlockFarmland::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		Block::onNeighborBlockChange(var1, var2, var3, var4, var5);
		Material* var6 = var1->getBlockMaterial(var2, var3 + 1, var4);
		if(var6->isSolid()) {
			var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
		}

}

int BlockFarmland::idDropped(int var1) {
		return Block::s_dirt->idDropped(0);
}
