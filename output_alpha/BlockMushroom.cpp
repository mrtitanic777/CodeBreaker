#include "stdafx.h"
// Auto-generated from BlockMushroom.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockMushroom.h"
#include "Common.h"
#include "World.h"
#include "Block.h"

BlockMushroom::BlockMushroom()
{
}

BlockMushroom::BlockMushroom(int var1, int var2)
	: BlockFlower(var1, var2)
{
		float var3 = 0.2F;
		setBlockBounds(0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, var3 * 2.0F, 0.5F + var3);
}

BlockMushroom::~BlockMushroom() {
	// No dynamic resources
}

bool BlockMushroom::canThisPlantGrowOnThisBlockID(int var1) {
		return Block::s_opaqueCubeLookup[var1];
}

bool BlockMushroom::canBlockStay(World* var1, int var2, int var3, int var4) {
		return var1->getBlockLightValue(var2, var3, var4) <= 13 && canThisPlantGrowOnThisBlockID(var1->getBlockId(var2, var3 - 1, var4));
}
