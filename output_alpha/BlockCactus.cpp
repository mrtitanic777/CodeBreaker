#include "stdafx.h"
// Auto-generated from BlockCactus.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockCactus.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockCactus::BlockCactus()
{
}

BlockCactus::BlockCactus(int var1, int var2)
	: Block(var1, var2, Material::s_cactus)
{
		setTickOnLoad(true);
}

BlockCactus::~BlockCactus() {
	// No dynamic resources
}

void BlockCactus::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var1->getBlockId(var2, var3 + 1, var4) == 0) {
			int var6;
			for(var6 = 1; var1->getBlockId(var2, var3 - var6, var4) == blockID; ++var6) {
			}

			if(var6 < 3) {
				int var7 = var1->getBlockMetadata(var2, var3, var4);
				if(var7 == 15) {
					var1->setBlockWithNotify(var2, var3 + 1, var4, blockID);
					var1->setBlockMetadataWithNotify(var2, var3, var4, 0);
				} else {
					var1->setBlockMetadataWithNotify(var2, var3, var4, var7 + 1);
				}
			}
		}

}

AxisAlignedBB BlockCactus::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		float var5 = 1.0F / 16.0F;
		return AxisAlignedBB::getBoundingBoxFromPool((double)((float)var2 + var5), (double)var3, (double)((float)var4 + var5), (double)((float)(var2 + 1) - var5), (double)((float)(var3 + 1) - var5), (double)((float)(var4 + 1) - var5));
}

AxisAlignedBB BlockCactus::getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		float var5 = 1.0F / 16.0F;
		return AxisAlignedBB::getBoundingBoxFromPool((double)((float)var2 + var5), (double)var3, (double)((float)var4 + var5), (double)((float)(var2 + 1) - var5), (double)(var3 + 1), (double)((float)(var4 + 1) - var5));
}

int BlockCactus::getBlockTextureFromSide(int var1) {
		return var1 == 1 ? blockIndexInTexture - 1 : (var1 == 0 ? blockIndexInTexture + 1 : blockIndexInTexture);
}

bool BlockCactus::renderAsNormalBlock() {
		return false;
}

bool BlockCactus::isOpaqueCube() {
		return false;
}

int BlockCactus::getRenderType() {
		return 13;
}

bool BlockCactus::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return !Block::canPlaceBlockAt(var1, var2, var3, var4) ? false : canBlockStay(var1, var2, var3, var4);
}

void BlockCactus::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(!canBlockStay(var1, var2, var3, var4)) {
			dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
			var1->setBlockWithNotify(var2, var3, var4, 0);
		}

}

bool BlockCactus::canBlockStay(World* var1, int var2, int var3, int var4) {
		if(var1->getBlockMaterial(var2 - 1, var3, var4)->func_878_a()) {
			return false;
		} else if(var1->getBlockMaterial(var2 + 1, var3, var4)->func_878_a()) {
			return false;
		} else if(var1->getBlockMaterial(var2, var3, var4 - 1)->func_878_a()) {
			return false;
		} else if(var1->getBlockMaterial(var2, var3, var4 + 1)->func_878_a()) {
			return false;
		} else {
			int var5 = var1->getBlockId(var2, var3 - 1, var4);
			return var5 == Block::s_cactus->blockID || var5 == Block::s_sand->blockID;
		}
}

void BlockCactus::onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5) {
		var5->canAttackEntity(NULL, 1);
}
