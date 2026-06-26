#include "stdafx.h"
// Auto-generated from BlockSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSand.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "BlockFire.h"
#include "EntityFallingSand.h"

bool BlockSand::s_fallInstantly = false;

BlockSand::BlockSand()
{
}

BlockSand::BlockSand(int var1, int var2)
	: Block(var1, var2, Material::s_sand)
{
}

BlockSand::~BlockSand() {
	// No dynamic resources
}

void BlockSand::onBlockAdded(World* var1, int var2, int var3, int var4) {
		var1->scheduleBlockUpdate(var2, var3, var4, blockID);
}

void BlockSand::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		var1->scheduleBlockUpdate(var2, var3, var4, blockID);
}

void BlockSand::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		tryToFall(var1, var2, var3, var4);
}

void BlockSand::tryToFall(World* var1, int var2, int var3, int var4) {
		if(canFallBelow(var1, var2, var3 - 1, var4) && var3 >= 0) {
			EntityFallingSand* var8 = new EntityFallingSand(var1, (float)var2 + 0.5F, (float)var3 + 0.5F, (float)var4 + 0.5F, blockID);
			if(s_fallInstantly) {
				while(!var8->isDead) {
					var8->onUpdate();
				}
			} else {
				var1->entityJoinedWorld(var8);
			}
		}

}

int BlockSand::tickRate() {
		return 3;
}

bool BlockSand::canFallBelow(World* var0, int var1, int var2, int var3) {
		int var4 = var0->getBlockId(var1, var2, var3);
		if(var4 == 0) {
			return true;
		} else if(var4 == Block::s_fire->blockID) {
			return true;
		} else {
			Material* var5 = Block::blocksList[var4]->m_blockMaterial;
			return var5 == Material::s_water ? true : var5 == Material::s_lava;
		}
}
