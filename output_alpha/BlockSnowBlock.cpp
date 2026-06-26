#include "stdafx.h"
// Auto-generated from BlockSnowBlock.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSnowBlock.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "EnumSkyBlock.h"
#include "Item.h"
#include "Material.h"

BlockSnowBlock::BlockSnowBlock()
{
}

BlockSnowBlock::BlockSnowBlock(int var1, int var2)
	: Block(var1, var2, Material::s_builtSnow)
{
		setTickOnLoad(true);
}

BlockSnowBlock::~BlockSnowBlock() {
	// No dynamic resources
}

int BlockSnowBlock::idDropped(int var1, Random* var2) {
		return Item::s_snowball->shiftedIndex;
}

int BlockSnowBlock::quantityDropped(Random* var1) {
		return 4;
}

void BlockSnowBlock::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var1->getSavedLightValue(EnumSkyBlock::s_Block, var2, var3, var4) > 11) {
			dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
			var1->setBlockWithNotify(var2, var3, var4, 0);
		}

}
