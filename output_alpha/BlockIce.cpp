#include "stdafx.h"
// Auto-generated from BlockIce.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockIce.h"
#include "Common.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "BlockBreakable.h"
#include "EnumSkyBlock.h"

BlockIce::BlockIce()
{
}

BlockIce::BlockIce(int var1, int var2)
	: BlockBreakable(var1, var2, Material::s_ice, false)
{
		slipperiness = 0.98F;
		setTickOnLoad(true);
}

BlockIce::~BlockIce() {
	// No dynamic resources
}

int BlockIce::func_234_g() {
		return 1;
}

bool BlockIce::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		return BlockBreakable::shouldSideBeRendered(var1, var2, var3, var4, 1 - var5);
}

void BlockIce::onBlockRemoval(World* var1, int var2, int var3, int var4) {
		Material* var5 = var1->getBlockMaterial(var2, var3 - 1, var4);
		if(var5->func_880_c() || var5->getIsLiquid()) {
			var1->setBlockWithNotify(var2, var3, var4, Block::s_waterStill->blockID);
		}

}

int BlockIce::quantityDropped(Random* var1) {
		return 0;
}

void BlockIce::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var1->getSavedLightValue(EnumSkyBlock::s_Block, var2, var3, var4) > 11 - Block::s_lightOpacity[blockID]) {
			dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
			var1->setBlockWithNotify(var2, var3, var4, Block::s_waterMoving->blockID);
		}

}
