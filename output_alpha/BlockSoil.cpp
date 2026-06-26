#include "stdafx.h"
// Auto-generated from BlockSoil.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSoil.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockSoil::BlockSoil()
{
}

BlockSoil::BlockSoil(int var1)
	: Block(var1, Material::s_ground)
{
		blockIndexInTexture = 87;
		setTickOnLoad(true);
		setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 15.0F / 16.0F, 1.0F);
		setLightOpacity(255);
}

BlockSoil::~BlockSoil() {
	// No dynamic resources
}

AxisAlignedBB BlockSoil::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB::getBoundingBoxFromPool((double)(var2 + 0), (double)(var3 + 0), (double)(var4 + 0), (double)(var2 + 1), (double)(var3 + 1), (double)(var4 + 1));
}

bool BlockSoil::isOpaqueCube() {
		return false;
}

bool BlockSoil::renderAsNormalBlock() {
		return false;
}

int BlockSoil::getBlockTextureFromSideAndMetadata(int var1, int var2) {
		return var1 == 1 && var2 > 0 ? blockIndexInTexture - 1 : (var1 == 1 ? blockIndexInTexture : 2);
}

void BlockSoil::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var5->nextInt(5) == 0) {
			if(func_274_i(var1, var2, var3, var4)) {
				var1->setBlockMetadataWithNotify(var2, var3, var4, 7);
			} else {
				int var6 = var1->getBlockMetadata(var2, var3, var4);
				if(var6 > 0) {
					var1->setBlockMetadataWithNotify(var2, var3, var4, var6 - 1);
				} else if(!func_275_h(var1, var2, var3, var4)) {
					var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
				}
			}
		}

}

void BlockSoil::onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5) {
		if(var1->rand->nextInt(4) == 0) {
			var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
		}

}

bool BlockSoil::func_275_h(World* var1, int var2, int var3, int var4) {
		unsigned char var5 = 0;

		for(int var6 = var2 - var5; var6 <= var2 + var5; ++var6) {
			for(int var7 = var4 - var5; var7 <= var4 + var5; ++var7) {
				if(var1->getBlockId(var6, var3 + 1, var7) == Block::s_crops->blockID) {
					return true;
				}
			}
		}

		return false;
}

bool BlockSoil::func_274_i(World* var1, int var2, int var3, int var4) {
		for(int var5 = var2 - 4; var5 <= var2 + 4; ++var5) {
			for(int var6 = var3; var6 <= var3 + 1; ++var6) {
				for(int var7 = var4 - 4; var7 <= var4 + 4; ++var7) {
					if(var1->getBlockMaterial(var5, var6, var7) == Material::s_water) {
						return true;
					}
				}
			}
		}

		return false;
}

void BlockSoil::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		Block::onNeighborBlockChange(var1, var2, var3, var4, var5);
		Material* var6 = var1->getBlockMaterial(var2, var3 + 1, var4);
		if(var6->func_878_a()) {
			var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
		}

}

int BlockSoil::idDropped(int var1, Random* var2) {
		return Block::s_dirt->idDropped(0, var2);
}
