#include "stdafx.h"
// Auto-generated from BlockTorch.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockTorch.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "MovingObjectPosition.h"
#include "Random.h"
#include "Vec3D.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockTorch::BlockTorch()
{
}

BlockTorch::BlockTorch(int var1, int var2)
	: Block(var1, var2, Material::s_circuits)
{
		setTickOnLoad(true);
}

BlockTorch::~BlockTorch() {
	// No dynamic resources
}

AxisAlignedBB BlockTorch::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

bool BlockTorch::isOpaqueCube() {
		return false;
}

bool BlockTorch::renderAsNormalBlock() {
		return false;
}

int BlockTorch::getRenderType() {
		return 2;
}

bool BlockTorch::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return var1->isBlockOpaqueCube(var2 - 1, var3, var4) ? true : (var1->isBlockOpaqueCube(var2 + 1, var3, var4) ? true : (var1->isBlockOpaqueCube(var2, var3, var4 - 1) ? true : (var1->isBlockOpaqueCube(var2, var3, var4 + 1) ? true : var1->isBlockOpaqueCube(var2, var3 - 1, var4))));
}

void BlockTorch::onBlockPlaced(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockMetadata(var2, var3, var4);
		if(var5 == 1 && var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
			var6 = 5;
		}

		if(var5 == 2 && var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
			var6 = 4;
		}

		if(var5 == 3 && var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
			var6 = 3;
		}

		if(var5 == 4 && var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
			var6 = 2;
		}

		if(var5 == 5 && var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
			var6 = 1;
		}

		var1->setBlockMetadataWithNotify(var2, var3, var4, var6);
}

void BlockTorch::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		Block::updateTick(var1, var2, var3, var4, var5);
		if(var1->getBlockMetadata(var2, var3, var4) == 0) {
			onBlockAdded(var1, var2, var3, var4);
		}

}

void BlockTorch::onBlockAdded(World* var1, int var2, int var3, int var4) {
		if(var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 1);
		} else if(var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 2);
		} else if(var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 3);
		} else if(var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 4);
		} else if(var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 5);
		}

		func_271_h(var1, var2, var3, var4);
}

void BlockTorch::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(func_271_h(var1, var2, var3, var4)) {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			bool var7 = false;
			if(!var1->isBlockOpaqueCube(var2 - 1, var3, var4) && var6 == 1) {
				var7 = true;
			}

			if(!var1->isBlockOpaqueCube(var2 + 1, var3, var4) && var6 == 2) {
				var7 = true;
			}

			if(!var1->isBlockOpaqueCube(var2, var3, var4 - 1) && var6 == 3) {
				var7 = true;
			}

			if(!var1->isBlockOpaqueCube(var2, var3, var4 + 1) && var6 == 4) {
				var7 = true;
			}

			if(!var1->isBlockOpaqueCube(var2, var3 - 1, var4) && var6 == 5) {
				var7 = true;
			}

			if(var7) {
				dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
				var1->setBlockWithNotify(var2, var3, var4, 0);
			}
		}

}

bool BlockTorch::func_271_h(World* var1, int var2, int var3, int var4) {
		if(!canPlaceBlockAt(var1, var2, var3, var4)) {
			dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
			var1->setBlockWithNotify(var2, var3, var4, 0);
			return false;
		} else {
			return true;
		}
}

MovingObjectPosition* BlockTorch::collisionRayTrace(World* var1, int var2, int var3, int var4, Vec3D* var5, Vec3D* var6) {
		int var7 = var1->getBlockMetadata(var2, var3, var4) & 7;
		float var8 = 0.15F;
		if(var7 == 1) {
			setBlockBounds(0.0F, 0.2F, 0.5F - var8, var8 * 2.0F, 0.8F, 0.5F + var8);
		} else if(var7 == 2) {
			setBlockBounds(1.0F - var8 * 2.0F, 0.2F, 0.5F - var8, 1.0F, 0.8F, 0.5F + var8);
		} else if(var7 == 3) {
			setBlockBounds(0.5F - var8, 0.2F, 0.0F, 0.5F + var8, 0.8F, var8 * 2.0F);
		} else if(var7 == 4) {
			setBlockBounds(0.5F - var8, 0.2F, 1.0F - var8 * 2.0F, 0.5F + var8, 0.8F, 1.0F);
		} else {
			var8 = 0.1F;
			setBlockBounds(0.5F - var8, 0.0F, 0.5F - var8, 0.5F + var8, 0.6F, 0.5F + var8);
		}

		return Block::collisionRayTrace(var1, var2, var3, var4, var5, var6);
}

void BlockTorch::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		int var6 = var1->getBlockMetadata(var2, var3, var4);
		double var7 = (double)((float)var2 + 0.5F);
		double var9 = (double)((float)var3 + 0.7F);
		double var11 = (double)((float)var4 + 0.5F);
		double var13 = (double)0.22F;
		double var15 = (double)0.27F;
		if(var6 == 1) {
			var1->spawnParticle("smoke", var7 - var15, var9 + var13, var11, 0.0f, 0.0f, 0.0f);
			var1->spawnParticle("flame", var7 - var15, var9 + var13, var11, 0.0f, 0.0f, 0.0f);
		} else if(var6 == 2) {
			var1->spawnParticle("smoke", var7 + var15, var9 + var13, var11, 0.0f, 0.0f, 0.0f);
			var1->spawnParticle("flame", var7 + var15, var9 + var13, var11, 0.0f, 0.0f, 0.0f);
		} else if(var6 == 3) {
			var1->spawnParticle("smoke", var7, var9 + var13, var11 - var15, 0.0f, 0.0f, 0.0f);
			var1->spawnParticle("flame", var7, var9 + var13, var11 - var15, 0.0f, 0.0f, 0.0f);
		} else if(var6 == 4) {
			var1->spawnParticle("smoke", var7, var9 + var13, var11 + var15, 0.0f, 0.0f, 0.0f);
			var1->spawnParticle("flame", var7, var9 + var13, var11 + var15, 0.0f, 0.0f, 0.0f);
		} else {
			var1->spawnParticle("smoke", var7, var9, var11, 0.0f, 0.0f, 0.0f);
			var1->spawnParticle("flame", var7, var9, var11, 0.0f, 0.0f, 0.0f);
		}

}
