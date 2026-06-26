#include "stdafx.h"
// Auto-generated from BlockLadder.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLadder.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockLadder::BlockLadder()
{
}

BlockLadder::BlockLadder(int var1, int var2)
	: Block(var1, var2, Material::s_circuits)
{
}

BlockLadder::~BlockLadder() {
	// No dynamic resources
}

AxisAlignedBB BlockLadder::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4);
		float var6 = 2.0F / 16.0F;
		if(var5 == 2) {
			setBlockBounds(0.0F, 0.0F, 1.0F - var6, 1.0F, 1.0F, 1.0F);
		}

		if(var5 == 3) {
			setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, var6);
		}

		if(var5 == 4) {
			setBlockBounds(1.0F - var6, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
		}

		if(var5 == 5) {
			setBlockBounds(0.0F, 0.0F, 0.0F, var6, 1.0F, 1.0F);
		}

		return Block::getCollisionBoundingBoxFromPool(var1, var2, var3, var4);
}

AxisAlignedBB BlockLadder::getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4);
		float var6 = 2.0F / 16.0F;
		if(var5 == 2) {
			setBlockBounds(0.0F, 0.0F, 1.0F - var6, 1.0F, 1.0F, 1.0F);
		}

		if(var5 == 3) {
			setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, var6);
		}

		if(var5 == 4) {
			setBlockBounds(1.0F - var6, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
		}

		if(var5 == 5) {
			setBlockBounds(0.0F, 0.0F, 0.0F, var6, 1.0F, 1.0F);
		}

		return Block::getSelectedBoundingBoxFromPool(var1, var2, var3, var4);
}

bool BlockLadder::isOpaqueCube() {
		return false;
}

bool BlockLadder::renderAsNormalBlock() {
		return false;
}

int BlockLadder::getRenderType() {
		return 8;
}

bool BlockLadder::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return var1->isBlockOpaqueCube(var2 - 1, var3, var4) ? true : (var1->isBlockOpaqueCube(var2 + 1, var3, var4) ? true : (var1->isBlockOpaqueCube(var2, var3, var4 - 1) ? true : var1->isBlockOpaqueCube(var2, var3, var4 + 1)));
}

void BlockLadder::onBlockPlaced(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockMetadata(var2, var3, var4);
		if((var6 == 0 || var5 == 2) && var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
			var6 = 2;
		}

		if((var6 == 0 || var5 == 3) && var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
			var6 = 3;
		}

		if((var6 == 0 || var5 == 4) && var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
			var6 = 4;
		}

		if((var6 == 0 || var5 == 5) && var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
			var6 = 5;
		}

		var1->setBlockMetadataWithNotify(var2, var3, var4, var6);
}

void BlockLadder::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockMetadata(var2, var3, var4);
		bool var7 = false;
		if(var6 == 2 && var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
			var7 = true;
		}

		if(var6 == 3 && var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
			var7 = true;
		}

		if(var6 == 4 && var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
			var7 = true;
		}

		if(var6 == 5 && var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
			var7 = true;
		}

		if(!var7) {
			dropBlockAsItem(var1, var2, var3, var4, var6);
			var1->setBlockWithNotify(var2, var3, var4, 0);
		}

		Block::onNeighborBlockChange(var1, var2, var3, var4, var5);
}

int BlockLadder::quantityDropped(Random* var1) {
		return 1;
}
