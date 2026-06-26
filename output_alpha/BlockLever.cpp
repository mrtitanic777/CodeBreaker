#include "stdafx.h"
// Auto-generated from BlockLever.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLever.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "EntityPlayer.h"
#include "IBlockAccess.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockLever::BlockLever()
{
}

BlockLever::BlockLever(int var1, int var2)
	: Block(var1, var2, Material::s_circuits)
{
}

BlockLever::~BlockLever() {
	// No dynamic resources
}

AxisAlignedBB BlockLever::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

bool BlockLever::isOpaqueCube() {
		return false;
}

bool BlockLever::renderAsNormalBlock() {
		return false;
}

int BlockLever::getRenderType() {
		return 12;
}

bool BlockLever::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return var1->isBlockOpaqueCube(var2 - 1, var3, var4) ? true : (var1->isBlockOpaqueCube(var2 + 1, var3, var4) ? true : (var1->isBlockOpaqueCube(var2, var3, var4 - 1) ? true : (var1->isBlockOpaqueCube(var2, var3, var4 + 1) ? true : var1->isBlockOpaqueCube(var2, var3 - 1, var4))));
}

void BlockLever::onBlockPlaced(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockMetadata(var2, var3, var4);
		int var7 = var6 & 8;
		var6 &= 7;
		if(var5 == 1 && var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
			var6 = 5 + var1->rand->nextInt(2);
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

		var1->setBlockMetadataWithNotify(var2, var3, var4, var6 + var7);
}

void BlockLever::onBlockAdded(World* var1, int var2, int var3, int var4) {
		if(var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 1);
		} else if(var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 2);
		} else if(var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 3);
		} else if(var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 4);
		} else if(var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 5 + var1->rand->nextInt(2));
		}

		checkIfAttachedToBlock(var1, var2, var3, var4);
}

void BlockLever::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(checkIfAttachedToBlock(var1, var2, var3, var4)) {
			int var6 = var1->getBlockMetadata(var2, var3, var4) & 7;
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

bool BlockLever::checkIfAttachedToBlock(World* var1, int var2, int var3, int var4) {
		if(!canPlaceBlockAt(var1, var2, var3, var4)) {
			dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
			var1->setBlockWithNotify(var2, var3, var4, 0);
			return false;
		} else {
			return true;
		}
}

void BlockLever::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4) & 7;
		float var6 = 3.0F / 16.0F;
		if(var5 == 1) {
			setBlockBounds(0.0F, 0.2F, 0.5F - var6, var6 * 2.0F, 0.8F, 0.5F + var6);
		} else if(var5 == 2) {
			setBlockBounds(1.0F - var6 * 2.0F, 0.2F, 0.5F - var6, 1.0F, 0.8F, 0.5F + var6);
		} else if(var5 == 3) {
			setBlockBounds(0.5F - var6, 0.2F, 0.0F, 0.5F + var6, 0.8F, var6 * 2.0F);
		} else if(var5 == 4) {
			setBlockBounds(0.5F - var6, 0.2F, 1.0F - var6 * 2.0F, 0.5F + var6, 0.8F, 1.0F);
		} else {
			var6 = 0.25F;
			setBlockBounds(0.5F - var6, 0.0F, 0.5F - var6, 0.5F + var6, 0.6F, 0.5F + var6);
		}

}

void BlockLever::onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		blockActivated(var1, var2, var3, var4, var5);
}

bool BlockLever::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		if(var1->m_multiplayerWorld) {
			return true;
		} else {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			int var7 = var6 & 7;
			int var8 = 8 - (var6 & 8);
			var1->setBlockMetadataWithNotify(var2, var3, var4, var7 + var8);
			var1->func_701_b(var2, var3, var4, var2, var3, var4);
			var1->playSoundEffect((double)var2 + 0.5f, (double)var3 + 0.5f, (double)var4 + 0.5f, "random.click", 0.3F, var8 > 0 ? 0.6F : 0.5F);
			var1->notifyBlocksOfNeighborChange(var2, var3, var4, blockID);
			if(var7 == 1) {
				var1->notifyBlocksOfNeighborChange(var2 - 1, var3, var4, blockID);
			} else if(var7 == 2) {
				var1->notifyBlocksOfNeighborChange(var2 + 1, var3, var4, blockID);
			} else if(var7 == 3) {
				var1->notifyBlocksOfNeighborChange(var2, var3, var4 - 1, blockID);
			} else if(var7 == 4) {
				var1->notifyBlocksOfNeighborChange(var2, var3, var4 + 1, blockID);
			} else {
				var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, blockID);
			}

			return true;
		}
}

void BlockLever::onBlockRemoval(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4);
		if((var5 & 8) > 0) {
			var1->notifyBlocksOfNeighborChange(var2, var3, var4, blockID);
			int var6 = var5 & 7;
			if(var6 == 1) {
				var1->notifyBlocksOfNeighborChange(var2 - 1, var3, var4, blockID);
			} else if(var6 == 2) {
				var1->notifyBlocksOfNeighborChange(var2 + 1, var3, var4, blockID);
			} else if(var6 == 3) {
				var1->notifyBlocksOfNeighborChange(var2, var3, var4 - 1, blockID);
			} else if(var6 == 4) {
				var1->notifyBlocksOfNeighborChange(var2, var3, var4 + 1, blockID);
			} else {
				var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, blockID);
			}
		}

		Block::onBlockRemoval(var1, var2, var3, var4);
}

bool BlockLever::isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		return (var1->getBlockMetadata(var2, var3, var4) & 8) > 0;
}

bool BlockLever::isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockMetadata(var2, var3, var4);
		if((var6 & 8) == 0) {
			return false;
		} else {
			int var7 = var6 & 7;
			return var7 == 5 && var5 == 1 ? true : (var7 == 4 && var5 == 2 ? true : (var7 == 3 && var5 == 3 ? true : (var7 == 2 && var5 == 4 ? true : var7 == 1 && var5 == 5)));
		}
}

bool BlockLever::canProvidePower() {
		return true;
}
