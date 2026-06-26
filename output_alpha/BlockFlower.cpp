#include "stdafx.h"
// Auto-generated from BlockFlower.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFlower.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "BlockGrass.h"

BlockFlower::BlockFlower()
{
}

BlockFlower::BlockFlower(int var1, int var2)
	: Block(var1, Material::s_plants)
{
		blockIndexInTexture = var2;
		setTickOnLoad(true);
		float var3 = 0.2F;
		setBlockBounds(0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, var3 * 3.0F, 0.5F + var3);
}

BlockFlower::~BlockFlower() {
	// No dynamic resources
}

bool BlockFlower::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return canThisPlantGrowOnThisBlockID(var1->getBlockId(var2, var3 - 1, var4));
}

bool BlockFlower::canThisPlantGrowOnThisBlockID(int var1) {
		return var1 == Block::s_grass->blockID || var1 == Block::s_dirt->blockID || var1 == Block::s_tilledField->blockID;
}

void BlockFlower::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		Block::onNeighborBlockChange(var1, var2, var3, var4, var5);
		func_268_h(var1, var2, var3, var4);
}

void BlockFlower::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		func_268_h(var1, var2, var3, var4);
}

void BlockFlower::func_268_h(World* var1, int var2, int var3, int var4) {
		if(!canBlockStay(var1, var2, var3, var4)) {
			dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
			var1->setBlockWithNotify(var2, var3, var4, 0);
		}

}

bool BlockFlower::canBlockStay(World* var1, int var2, int var3, int var4) {
		return (var1->getBlockLightValue(var2, var3, var4) >= 8 || var1->canBlockSeeTheSky(var2, var3, var4)) && canThisPlantGrowOnThisBlockID(var1->getBlockId(var2, var3 - 1, var4));
}

AxisAlignedBB BlockFlower::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

bool BlockFlower::isOpaqueCube() {
		return false;
}

bool BlockFlower::renderAsNormalBlock() {
		return false;
}

int BlockFlower::getRenderType() {
		return 1;
}
