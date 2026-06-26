#include "stdafx.h"
// Auto-generated from BlockReed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockReed.h"
#include "Common.h"
#include "BlockGrass.h"
#include "AxisAlignedBB.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "Item.h"

BlockReed::BlockReed()
{
}

BlockReed::BlockReed(int var1, int var2)
	: Block(var1, Material::s_plants)
{
		blockIndexInTexture = var2;
		float var3 = 6.0F / 16.0F;
		setBlockBounds(0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, 1.0F, 0.5F + var3);
		setTickOnLoad(true);
}

BlockReed::~BlockReed() {
	// No dynamic resources
}

void BlockReed::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
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

bool BlockReed::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockId(var2, var3 - 1, var4);
		return var5 == blockID ? true : (var5 != Block::s_grass->blockID && var5 != Block::s_dirt->blockID ? false : (var1->getBlockMaterial(var2 - 1, var3 - 1, var4) == Material::s_water ? true : (var1->getBlockMaterial(var2 + 1, var3 - 1, var4) == Material::s_water ? true : (var1->getBlockMaterial(var2, var3 - 1, var4 - 1) == Material::s_water ? true : var1->getBlockMaterial(var2, var3 - 1, var4 + 1) == Material::s_water))));
}

void BlockReed::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		checkBlockCoordValid(var1, var2, var3, var4);
}

void BlockReed::checkBlockCoordValid(World* var1, int var2, int var3, int var4) {
		if(!canBlockStay(var1, var2, var3, var4)) {
			dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
			var1->setBlockWithNotify(var2, var3, var4, 0);
		}

}

bool BlockReed::canBlockStay(World* var1, int var2, int var3, int var4) {
		return canPlaceBlockAt(var1, var2, var3, var4);
}

AxisAlignedBB BlockReed::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

int BlockReed::idDropped(int var1, Random* var2) {
		return Item::s_reed->shiftedIndex;
}

bool BlockReed::isOpaqueCube() {
		return false;
}

bool BlockReed::renderAsNormalBlock() {
		return false;
}

int BlockReed::getRenderType() {
		return 1;
}
