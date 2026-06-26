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

BlockTorch::BlockTorch()
{
}

BlockTorch::BlockTorch(int var1, int var2)
	: Block(50, 80, Material.circuits)
{
}

BlockTorch::~BlockTorch() {
	// No dynamic resources
}

AxisAlignedBB BlockTorch::getCollisionBoundingBoxFromPool(int var1, int var2, int var3) {
		return NULL;
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
		return var1->isBlockNormalCube(var2 - 1, var3, var4) ? true : (var1->isBlockNormalCube(var2 + 1, var3, var4) ? true : (var1->isBlockNormalCube(var2, var3, var4 - 1) ? true : (var1->isBlockNormalCube(var2, var3, var4 + 1) ? true : var1->isBlockNormalCube(var2, var3 - 1, var4))));
}

void BlockTorch::onBlockPlaced(World* var1, int var2, int var3, int var4, int var5) {
		unsigned char var6 = var1->getBlockMetadata(var2, var3, var4);
		if(var5 == 1 && var1->isBlockNormalCube(var2, var3 - 1, var4)) {
			var6 = 5;
		}

		if(var5 == 2 && var1->isBlockNormalCube(var2, var3, var4 + 1)) {
			var6 = 4;
		}

		if(var5 == 3 && var1->isBlockNormalCube(var2, var3, var4 - 1)) {
			var6 = 3;
		}

		if(var5 == 4 && var1->isBlockNormalCube(var2 + 1, var3, var4)) {
			var6 = 2;
		}

		if(var5 == 5 && var1->isBlockNormalCube(var2 - 1, var3, var4)) {
			var6 = 1;
		}

		var1->setBlockMetadata(var2, var3, var4, var6);
}

void BlockTorch::onBlockAdded(World* var1, int var2, int var3, int var4) {
		if(var1->isBlockNormalCube(var2 - 1, var3, var4)) {
			var1->setBlockMetadata(var2, var3, var4, 1);
		} else if(var1->isBlockNormalCube(var2 + 1, var3, var4)) {
			var1->setBlockMetadata(var2, var3, var4, 2);
		} else if(var1->isBlockNormalCube(var2, var3, var4 - 1)) {
			var1->setBlockMetadata(var2, var3, var4, 3);
		} else if(var1->isBlockNormalCube(var2, var3, var4 + 1)) {
			var1->setBlockMetadata(var2, var3, var4, 4);
		} else if(var1->isBlockNormalCube(var2, var3 - 1, var4)) {
			var1->setBlockMetadata(var2, var3, var4, 5);
		}

		dropTorchIfCantStay(var1, var2, var3, var4);
}

void BlockTorch::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(dropTorchIfCantStay(var1, var2, var3, var4)) {
			unsigned char var7 = var1->getBlockMetadata(var2, var3, var4);
			bool var6 = false;
			if(!var1->isBlockNormalCube(var2 - 1, var3, var4) && var7 == 1) {
				var6 = true;
			}

			if(!var1->isBlockNormalCube(var2 + 1, var3, var4) && var7 == 2) {
				var6 = true;
			}

			if(!var1->isBlockNormalCube(var2, var3, var4 - 1) && var7 == 3) {
				var6 = true;
			}

			if(!var1->isBlockNormalCube(var2, var3, var4 + 1) && var7 == 4) {
				var6 = true;
			}

			if(!var1->isBlockNormalCube(var2, var3 - 1, var4 + 1) && var7 == 5) {
				var6 = true;
			}

			if(var6) {
				dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
				var1->setBlockWithNotify(var2, var3, var4, 0);
			}
		}

}

bool BlockTorch::dropTorchIfCantStay(World* var1, int var2, int var3, int var4) {
		if(!canPlaceBlockAt(var1, var2, var3, var4)) {
			dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
			var1->setBlockWithNotify(var2, var3, var4, 0);
			return false;
		} else {
			return true;
		}
}

MovingObjectPosition* BlockTorch::collisionRayTrace(World* var1, int var2, int var3, int var4, Vec3D* var5, Vec3D* var6) {
		unsigned char var7 = var1->getBlockMetadata(var2, var3, var4);
		if(var7 == 1) {
			setBlockBounds(0.0F, 0.2F, 0.35F, 0.3F, 0.8F, 0.65F);
		} else if(var7 == 2) {
			setBlockBounds(0.7F, 0.2F, 0.35F, 1.0F, 0.8F, 0.65F);
		} else if(var7 == 3) {
			setBlockBounds(0.35F, 0.2F, 0.0F, 0.65F, 0.8F, 0.3F);
		} else if(var7 == 4) {
			setBlockBounds(0.35F, 0.2F, 0.7F, 0.65F, 0.8F, 1.0F);
		} else {
			setBlockBounds(0.4F, 0.0F, 0.4F, 0.6F, 0.6F, 0.6F);
		}

		return Block::collisionRayTrace(var1, var2, var3, var4, var5, var6);
}

void BlockTorch::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		unsigned char var9 = var1->getBlockMetadata(var2, var3, var4);
		float var6 = (float)var2 + 0.5F;
		float var7 = (float)var3 + 0.7F;
		float var8 = (float)var4 + 0.5F;
		if(var9 == 1) {
			var1->spawnParticle("smoke", var6 - 0.27F, var7 + 0.22F, var8, 0.0F, 0.0F, 0.0F);
			var1->spawnParticle("flame", var6 - 0.27F, var7 + 0.22F, var8, 0.0F, 0.0F, 0.0F);
		} else if(var9 == 2) {
			var1->spawnParticle("smoke", var6 + 0.27F, var7 + 0.22F, var8, 0.0F, 0.0F, 0.0F);
			var1->spawnParticle("flame", var6 + 0.27F, var7 + 0.22F, var8, 0.0F, 0.0F, 0.0F);
		} else if(var9 == 3) {
			var1->spawnParticle("smoke", var6, var7 + 0.22F, var8 - 0.27F, 0.0F, 0.0F, 0.0F);
			var1->spawnParticle("flame", var6, var7 + 0.22F, var8 - 0.27F, 0.0F, 0.0F, 0.0F);
		} else if(var9 == 4) {
			var1->spawnParticle("smoke", var6, var7 + 0.22F, var8 + 0.27F, 0.0F, 0.0F, 0.0F);
			var1->spawnParticle("flame", var6, var7 + 0.22F, var8 + 0.27F, 0.0F, 0.0F, 0.0F);
		} else {
			var1->spawnParticle("smoke", var6, var7, var8, 0.0F, 0.0F, 0.0F);
			var1->spawnParticle("flame", var6, var7, var8, 0.0F, 0.0F, 0.0F);
		}
}
