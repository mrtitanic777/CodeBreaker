#include "stdafx.h"
// Auto-generated from WorldGenCactus.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenCactus.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"

WorldGenCactus::WorldGenCactus()
{
}

WorldGenCactus::~WorldGenCactus() {
	// No dynamic resources
}

bool WorldGenCactus::generate(World* var1, Random* var2, int var3, int var4, int var5) {
		for(int var6 = 0; var6 < 10; ++var6) {
			int var7 = var3 + var2->nextInt(8) - var2->nextInt(8);
			int var8 = var4 + var2->nextInt(4) - var2->nextInt(4);
			int var9 = var5 + var2->nextInt(8) - var2->nextInt(8);
			if(var1->getBlockId(var7, var8, var9) == 0) {
				int var10 = 1 + var2->nextInt(var2->nextInt(3) + 1);

				for(int var11 = 0; var11 < var10; ++var11) {
					if(Block::s_cactus->canBlockStay(var1, var7, var8 + var11, var9)) {
						var1->setBlock(var7, var8 + var11, var9, Block::s_cactus->blockID);
					}
				}
			}
		}

		return true;
}
