#include "stdafx.h"
// Auto-generated from BlockStationary.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockStationary.h"
#include "Common.h"
#include "Material.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFluids.h"

BlockStationary::BlockStationary()
{
}

BlockStationary::BlockStationary(int var1, Material* var2)
	: BlockFluids(var1, var2)
{
		setTickOnLoad(false);
		if(var2 == Material::s_lava) {
			setTickOnLoad(true);
		}

}

BlockStationary::~BlockStationary() {
	// No dynamic resources
}

void BlockStationary::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		BlockFluids::onNeighborBlockChange(var1, var2, var3, var4, var5);
		if(var1->getBlockId(var2, var3, var4) == blockID) {
			func_15234_j(var1, var2, var3, var4);
		}

}

void BlockStationary::func_15234_j(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4);
		var1->m_field_1043_h = true;
		var1->setBlockAndMetadata(var2, var3, var4, blockID - 1, var5);
		var1->func_701_b(var2, var3, var4, var2, var3, var4);
		var1->scheduleBlockUpdate(var2, var3, var4, blockID - 1);
		var1->m_field_1043_h = false;
}

void BlockStationary::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(m_blockMaterial == Material::s_lava) {
			int var6 = var5->nextInt(3);

			for(int var7 = 0; var7 < var6; ++var7) {
				var2 += var5->nextInt(3) - 1;
				++var3;
				var4 += var5->nextInt(3) - 1;
				int var8 = var1->getBlockId(var2, var3, var4);
				if(var8 == 0) {
					if(func_301_k(var1, var2 - 1, var3, var4) || func_301_k(var1, var2 + 1, var3, var4) || func_301_k(var1, var2, var3, var4 - 1) || func_301_k(var1, var2, var3, var4 + 1) || func_301_k(var1, var2, var3 - 1, var4) || func_301_k(var1, var2, var3 + 1, var4)) {
						var1->setBlockWithNotify(var2, var3, var4, Block::s_fire->blockID);
						return;
					}
				} else if(Block::blocksList[var8]->m_blockMaterial->func_880_c()) {
					return;
				}
			}
		}

}

bool BlockStationary::func_301_k(World* var1, int var2, int var3, int var4) {
		return var1->getBlockMaterial(var2, var3, var4)->getBurning();
}
