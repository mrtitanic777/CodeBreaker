#include "stdafx.h"
// Auto-generated from BlockSponge.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSponge.h"
#include "Common.h"
#include "World.h"
#include "Block.h"

BlockSponge::BlockSponge()
{
}

BlockSponge::BlockSponge(int var1)
	: Block(19, Material.sponge)
{
		blockIndexInTexture = 48;
}

BlockSponge::~BlockSponge() {
	// No dynamic resources
}

void BlockSponge::onBlockAdded(World* var1, int var2, int var3, int var4) {
		for(int var5 = var2 - 2; var5 <= var2 + 2; ++var5) {
			for(int var6 = var3 - 2; var6 <= var3 + 2; ++var6) {
				for(int var7 = var4 - 2; var7 <= var4 + 2; ++var7) {
					if(var1->isWater(var5, var6, var7)) {
						var1->setBlock(var5, var6, var7, 0);
					}
				}
			}
		}

}

void BlockSponge::onBlockRemoval(World* var1, int var2, int var3, int var4) {
		for(int var5 = var2 - 2; var5 <= var2 + 2; ++var5) {
			for(int var6 = var3 - 2; var6 <= var3 + 2; ++var6) {
				for(int var7 = var4 - 2; var7 <= var4 + 2; ++var7) {
					var1->notifyBlocksOfNeighborChange(var5, var6, var7, var1->getBlockId(var5, var6, var7));
				}
			}
		}

}
