#include "stdafx.h"
// Auto-generated from WorldGenReed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "WorldGenReed.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Material.h"
#include "Block.h"

WorldGenReed::WorldGenReed()
{
}

WorldGenReed::~WorldGenReed() {
	// No dynamic resources
}

bool WorldGenReed::generate(World* var1, Random* var2, int var3, int var4, int var5) {
		for(int var6 = 0; var6 < 20; ++var6) {
			int var7 = var3 + var2->nextInt(4) - var2->nextInt(4);
			int var8 = var4;
			int var9 = var5 + var2->nextInt(4) - var2->nextInt(4);
			if(var1->getBlockId(var7, var4, var9) == 0 && (var1->getBlockMaterial(var7 - 1, var4 - 1, var9) == Material::s_water || var1->getBlockMaterial(var7 + 1, var4 - 1, var9) == Material::s_water || var1->getBlockMaterial(var7, var4 - 1, var9 - 1) == Material::s_water || var1->getBlockMaterial(var7, var4 - 1, var9 + 1) == Material::s_water)) {
				int var10 = 2 + var2->nextInt(var2->nextInt(3) + 1);

				for(int var11 = 0; var11 < var10; ++var11) {
					if(Block::s_reed->canBlockStay(var1, var7, var8 + var11, var9)) {
						var1->setBlock(var7, var8 + var11, var9, Block::s_reed->blockID);
					}
				}
			}
		}

		return true;
}
