#include "stdafx.h"
// Auto-generated from BlockTNT.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockTNT.h"
#include "Common.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "EntityTNT.h"
#include "Material.h"

BlockTNT::BlockTNT()
{
}

BlockTNT::BlockTNT(int var1, int var2)
	: Block(46, 8, Material::s_tnt)
{
}

BlockTNT::~BlockTNT() {
	// No dynamic resources
}

int BlockTNT::getBlockTextureFromSide(int var1) {
		return var1 == 0 ? blockIndexInTexture + 2 : (var1 == 1 ? blockIndexInTexture + 1 : blockIndexInTexture);
}

int BlockTNT::quantityDropped(Random* var1) {
		return 0;
}

void BlockTNT::onBlockDestroyedByExplosion(World* var1, int var2, int var3, int var4) {
		EntityTNT* var5 = new EntityTNT(var1, (float)var2 + 0.5F, (float)var3 + 0.5F, (float)var4 + 0.5F);
		var5->m_fuse = var1->m_random->nextInt(var5->m_fuse / 4) + var5->m_fuse / 8;
		var1->spawnEntityInWorld(var5);
}

void BlockTNT::onBlockDestroyedByPlayer(World* var1, int var2, int var3, int var4, int var5) {
		EntityTNT* var6 = new EntityTNT(var1, (float)var2 + 0.5F, (float)var3 + 0.5F, (float)var4 + 0.5F);
		var1->spawnEntityInWorld(var6);
		var1->a(var6, "random.m_fuse", 1.0F, 1.0F);
}
