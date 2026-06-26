#include "stdafx.h"
// Auto-generated from BlockSponge.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSponge.h"
#include "Common.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockSponge::BlockSponge()
{
}

BlockSponge::BlockSponge(int var1)
	: Block(var1, Material::s_sponge)
{
		blockIndexInTexture = 48;
}

BlockSponge::~BlockSponge() {
	// No dynamic resources
}

void BlockSponge::onBlockAdded(World* var1, int var2, int var3, int var4) {
		unsigned char var5 = 2;

		for(int var6 = var2 - var5; var6 <= var2 + var5; ++var6) {
			for(int var7 = var3 - var5; var7 <= var3 + var5; ++var7) {
				for(int var8 = var4 - var5; var8 <= var4 + var5; ++var8) {
					if(var1->getBlockMaterial(var6, var7, var8) == Material::s_water) {
					}
				}
			}
		}

}

void BlockSponge::onBlockRemoval(World* var1, int var2, int var3, int var4) {
		unsigned char var5 = 2;

		for(int var6 = var2 - var5; var6 <= var2 + var5; ++var6) {
			for(int var7 = var3 - var5; var7 <= var3 + var5; ++var7) {
				for(int var8 = var4 - var5; var8 <= var4 + var5; ++var8) {
					var1->notifyBlocksOfNeighborChange(var6, var7, var8, var1->getBlockId(var6, var7, var8));
				}
			}
		}

}
