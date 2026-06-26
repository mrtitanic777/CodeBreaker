#include "stdafx.h"
// Auto-generated from BlockButton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockButton.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "EntityPlayer.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockButton::BlockButton()
{
}

BlockButton::BlockButton(int var1, int var2)
	: Block(var1, var2, Material::s_circuits)
{
		setTickOnLoad(true);
}

BlockButton::~BlockButton() {
	// No dynamic resources
}

AxisAlignedBB BlockButton::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

int BlockButton::tickRate() {
		return 20;
}

bool BlockButton::isOpaqueCube() {
		return false;
}

bool BlockButton::renderAsNormalBlock() {
		return false;
}

bool BlockButton::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return var1->isBlockOpaqueCube(var2 - 1, var3, var4) ? true : (var1->isBlockOpaqueCube(var2 + 1, var3, var4) ? true : (var1->isBlockOpaqueCube(var2, var3, var4 - 1) ? true : var1->isBlockOpaqueCube(var2, var3, var4 + 1)));
}

void BlockButton::onBlockPlaced(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockMetadata(var2, var3, var4);
		int var7 = var6 & 8;
		var6 &= 7;
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

void BlockButton::onBlockAdded(World* var1, int var2, int var3, int var4) {
		if(var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 1);
		} else if(var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 2);
		} else if(var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 3);
		} else if(var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 4);
		}

		func_305_h(var1, var2, var3, var4);
}

void BlockButton::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(func_305_h(var1, var2, var3, var4)) {
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

			if(var7) {
				dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
				var1->setBlockWithNotify(var2, var3, var4, 0);
			}
		}

}

bool BlockButton::func_305_h(World* var1, int var2, int var3, int var4) {
		if(!canPlaceBlockAt(var1, var2, var3, var4)) {
			dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
			var1->setBlockWithNotify(var2, var3, var4, 0);
			return false;
		} else {
			return true;
		}
}

void BlockButton::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4);
		int var6 = var5 & 7;
		bool var7 = (var5 & 8) > 0;
		float var8 = 6.0F / 16.0F;
		float var9 = 10.0F / 16.0F;
		float var10 = 3.0F / 16.0F;
		float var11 = 2.0F / 16.0F;
		if(var7) {
			var11 = 1.0F / 16.0F;
		}

		if(var6 == 1) {
			setBlockBounds(0.0F, var8, 0.5F - var10, var11, var9, 0.5F + var10);
		} else if(var6 == 2) {
			setBlockBounds(1.0F - var11, var8, 0.5F - var10, 1.0F, var9, 0.5F + var10);
		} else if(var6 == 3) {
			setBlockBounds(0.5F - var10, var8, 0.0F, 0.5F + var10, var9, var11);
		} else if(var6 == 4) {
			setBlockBounds(0.5F - var10, var8, 1.0F - var11, 0.5F + var10, var9, 1.0F);
		}

}

void BlockButton::onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		blockActivated(var1, var2, var3, var4, var5);
}

bool BlockButton::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5) {
		if(var1->m_multiplayerWorld) {
			return true;
		} else {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			int var7 = var6 & 7;
			int var8 = 8 - (var6 & 8);
			if(var8 == 0) {
				return true;
			} else {
				var1->setBlockMetadataWithNotify(var2, var3, var4, var7 + var8);
				var1->func_701_b(var2, var3, var4, var2, var3, var4);
				var1->playSoundEffect((double)var2 + 0.5f, (double)var3 + 0.5f, (double)var4 + 0.5f, "random.click", 0.3F, 0.6F);
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

				var1->scheduleBlockUpdate(var2, var3, var4, blockID);
				return true;
			}
		}
}

void BlockButton::onBlockRemoval(World* var1, int var2, int var3, int var4) {
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

bool BlockButton::isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		return (var1->getBlockMetadata(var2, var3, var4) & 8) > 0;
}

bool BlockButton::isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5) {
		int var6 = var1->getBlockMetadata(var2, var3, var4);
		if((var6 & 8) == 0) {
			return false;
		} else {
			int var7 = var6 & 7;
			return var7 == 5 && var5 == 1 ? true : (var7 == 4 && var5 == 2 ? true : (var7 == 3 && var5 == 3 ? true : (var7 == 2 && var5 == 4 ? true : var7 == 1 && var5 == 5)));
		}
}

bool BlockButton::canProvidePower() {
		return true;
}

void BlockButton::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(!var1->m_multiplayerWorld) {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			if((var6 & 8) != 0) {
				var1->setBlockMetadataWithNotify(var2, var3, var4, var6 & 7);
				var1->notifyBlocksOfNeighborChange(var2, var3, var4, blockID);
				int var7 = var6 & 7;
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

				var1->playSoundEffect((double)var2 + 0.5f, (double)var3 + 0.5f, (double)var4 + 0.5f, "random.click", 0.3F, 0.5F);
				var1->func_701_b(var2, var3, var4, var2, var3, var4);
			}
		}
}

void BlockButton::func_237_e() {
		float var1 = 3.0F / 16.0F;
		float var2 = 2.0F / 16.0F;
		float var3 = 2.0F / 16.0F;
		setBlockBounds(0.5F - var1, 0.5F - var2, 0.5F - var3, 0.5F + var1, 0.5F + var2, 0.5F + var3);
}
