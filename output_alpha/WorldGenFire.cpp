#include "stdafx.h"
// Auto-generated from WorldGenFire.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenFire.h"
#include "Common.h"
#include "BlockFire.h"
#include "Random.h"
#include "World.h"
#include "Block.h"

WorldGenFire::WorldGenFire()
{
}

WorldGenFire::~WorldGenFire() {
	// No dynamic resources
}

bool WorldGenFire::generate(World* var1, Random* var2, int var3, int var4, int var5) {
		for(int var6 = 0; var6 < 64; ++var6) {
			int var7 = var3 + var2->nextInt(8) - var2->nextInt(8);
			int var8 = var4 + var2->nextInt(4) - var2->nextInt(4);
			int var9 = var5 + var2->nextInt(8) - var2->nextInt(8);
			if(var1->getBlockId(var7, var8, var9) == 0 && var1->getBlockId(var7, var8 - 1, var9) == Block::s_bloodStone->blockID) {
				var1->setBlockWithNotify(var7, var8, var9, Block::s_fire->blockID);
			}
		}

		return true;
}
