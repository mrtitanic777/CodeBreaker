#include "stdafx.h"
// Auto-generated from BlockSapling.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSapling.h"
#include "Common.h"
#include "Random.h"
#include "World.h"

BlockSapling::BlockSapling()
{
}

BlockSapling::BlockSapling(int var1, int var2)
	: BlockFlower(6, 15)
{
		setBlockBounds(10.0F * 0.01F, 0.0F, 10.0F * 0.01F, 0.9F, 0.8F, 0.9F);
}

BlockSapling::~BlockSapling() {
	// No dynamic resources
}

void BlockSapling::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		BlockFlower::updateTick(var1, var2, var3, var4, var5);
		if(var1->getBlockLightValue(var2, var3 + 1, var4) >= 9 && var5->nextInt(5) == 0) {
			var1->setTileNoUpdate(var2, var3, var4, 0);
			if(!var1->growTrees(var2, var3, var4)) {
				var1->setTileNoUpdate(var2, var3, var4, blockID);
			}
		}

}
