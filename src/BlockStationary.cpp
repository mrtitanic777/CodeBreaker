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

BlockStationary::BlockStationary()
{
}

BlockStationary::BlockStationary(int var1, Material* var2)
	: BlockFluid(var1, var2)
{
		movingId = var1 - 1;
		stillId = var1;
		setTickOnLoad(false);
}

BlockStationary::~BlockStationary() {
	// No dynamic resources
}

void BlockStationary::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
	// TODO: implement
}

void BlockStationary::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		bool var6 = false;
		if(canFlow(var1, var2, var3 - 1, var4)) {
			var6 = true;
		}

		if(!var6 && canFlow(var1, var2 - 1, var3, var4)) {
			var6 = true;
		}

		if(!var6 && canFlow(var1, var2 + 1, var3, var4)) {
			var6 = true;
		}

		if(!var6 && canFlow(var1, var2, var3, var4 - 1)) {
			var6 = true;
		}

		if(!var6 && canFlow(var1, var2, var3, var4 + 1)) {
			var6 = true;
		}

		if(var5 != 0) {
			Material* var7 = Block::blocksList[var5].material;
			if(material == Material::s_water && var7 == Material::s_lava || var7 == Material::s_water && material == Material::s_lava) {
				var1->setBlockWithNotify(var2, var3, var4, Block::s_stone->blockID);
				return;
			}
		}

		if(Block::s_fire->getChanceOfNeighborsEncouragingFire(var5)) {
			var6 = true;
		}

		if(var6) {
			var1->setTileNoUpdate(var2, var3, var4, movingId);
			var1->scheduleBlockUpdate(var2, var3, var4, movingId);
		}

}
