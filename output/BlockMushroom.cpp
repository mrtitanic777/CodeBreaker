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
		setBlockBounds(0.3F, 0.0F, 0.3F, 0.7F, 0.4F, 0.7F);
}

BlockMushroom::~BlockMushroom() {
	// No dynamic resources
}

bool BlockMushroom::canThisPlantGrowOnThisBlockID(int var1) {
		return Block::s_opaqueCubeLookup[var1];
}

bool BlockMushroom::canBlockStay(World* var1, int var2, int var3, int var4) {
		if(var1->getBlockLightValue(var2, var3, var4) <= 13) {
			var2 = var1->getBlockId(var2, var3 - 1, var4);
			if(Block::s_opaqueCubeLookup[var2]) {
				return true;
			}
		}

		return false;
}
