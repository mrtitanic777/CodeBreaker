#include "stdafx.h"
// Auto-generated from WorldGenFlowers.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenFlowers.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "BlockFlower.h"

WorldGenFlowers::WorldGenFlowers()
	: m_plantBlockId(0)
{
}

WorldGenFlowers::WorldGenFlowers(int var1)
	: m_plantBlockId(0)
{
		m_plantBlockId = var1;
}

WorldGenFlowers::~WorldGenFlowers() {
	// No dynamic resources
}

bool WorldGenFlowers::generate(World* var1, Random* var2, int var3, int var4, int var5) {
		for(int var6 = 0; var6 < 64; ++var6) {
			int var7 = var3 + var2->nextInt(8) - var2->nextInt(8);
			int var8 = var4 + var2->nextInt(4) - var2->nextInt(4);
			int var9 = var5 + var2->nextInt(8) - var2->nextInt(8);
			if(var1->getBlockId(var7, var8, var9) == 0 && ((BlockFlower*)Block::blocksList[m_plantBlockId])->canBlockStay(var1, var7, var8, var9)) {
				var1->setBlock(var7, var8, var9, m_plantBlockId);
			}
		}

		return true;
}
