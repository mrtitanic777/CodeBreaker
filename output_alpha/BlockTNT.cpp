#include "stdafx.h"
// Auto-generated from BlockTNT.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockTNT.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"
#include "EntityTNTPrimed.h"

BlockTNT::BlockTNT()
{
}

BlockTNT::BlockTNT(int var1, int var2)
	: Block(var1, var2, Material::s_tnt)
{
}

BlockTNT::~BlockTNT() {
	// No dynamic resources
}

int BlockTNT::getBlockTextureFromSide(int var1) {
		return var1 == 0 ? blockIndexInTexture + 2 : (var1 == 1 ? blockIndexInTexture + 1 : blockIndexInTexture);
}

void BlockTNT::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(var5 > 0 && Block::blocksList[var5]->canProvidePower() && var1->isBlockIndirectlyGettingPowered(var2, var3, var4)) {
			onBlockDestroyedByPlayer(var1, var2, var3, var4, 0);
			var1->setBlockWithNotify(var2, var3, var4, 0);
		}

}

int BlockTNT::quantityDropped(Random* var1) {
		return 0;
}

void BlockTNT::onBlockDestroyedByExplosion(World* var1, int var2, int var3, int var4) {
		EntityTNTPrimed* var5 = new EntityTNTPrimed(var1, (double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F));
		var5->m_fuse = var1->rand->nextInt(var5->m_fuse / 4) + var5->m_fuse / 8;
		var1->entityJoinedWorld(var5);
}

void BlockTNT::onBlockDestroyedByPlayer(World* var1, int var2, int var3, int var4, int var5) {
		if(!var1->m_multiplayerWorld) {
			EntityTNTPrimed* var6 = new EntityTNTPrimed(var1, (double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F));
			var1->entityJoinedWorld(var6);
			var1->playSoundAtEntity(var6, "random.m_fuse", 1.0F, 1.0F);
		}
}
