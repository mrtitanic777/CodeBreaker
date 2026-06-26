#include "stdafx.h"
// Auto-generated from BlockRedstoneWire.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockRedstoneWire.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Item.h"
#include "Material.h"

BlockRedstoneWire::BlockRedstoneWire()
	: m_wiresProvidePower(true)
{
}

BlockRedstoneWire::BlockRedstoneWire(int var1, int var2)
	: Block(var1, var2, Material::s_circuits)
	, m_wiresProvidePower(true)
{
		setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F / 16.0F, 1.0F);
}

BlockRedstoneWire::~BlockRedstoneWire() {
	// No dynamic resources
}

int BlockRedstoneWire::getBlockTextureFromSideAndMetadata(int var1, int var2) {
		return blockIndexInTexture + (var2 > 0 ? 16 : 0);
}

AxisAlignedBB BlockRedstoneWire::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

bool BlockRedstoneWire::isOpaqueCube() {
		return false;
}

bool BlockRedstoneWire::renderAsNormalBlock() {
		return false;
}

int BlockRedstoneWire::getRenderType() {
		return 5;
}

bool BlockRedstoneWire::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return var1->isBlockOpaqueCube(var2, var3 - 1, var4);
}

void BlockRedstoneWire::updateAndPropagateCurrentStrength(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4);
		int var6 = 0;
		m_wiresProvidePower = false;
		bool var7 = var1->isBlockIndirectlyGettingPowered(var2, var3, var4);
		m_wiresProvidePower = true;
		int var8;
		int var9;
		int var10;
		if(var7) {
			var6 = 15;
		} else {
			for(var8 = 0; var8 < 4; ++var8) {
				var9 = var2;
				var10 = var4;
				if(var8 == 0) {
					var9 = var2 - 1;
				}

				if(var8 == 1) {
					++var9;
				}

				if(var8 == 2) {
					var10 = var4 - 1;
				}

				if(var8 == 3) {
					++var10;
				}

				var6 = getMaxCurrentStrength(var1, var9, var3, var10, var6);
				if(var1->isBlockOpaqueCube(var9, var3, var10) && !var1->isBlockOpaqueCube(var2, var3 + 1, var4)) {
					var6 = getMaxCurrentStrength(var1, var9, var3 + 1, var10, var6);
				} else if(!var1->isBlockOpaqueCube(var9, var3, var10)) {
					var6 = getMaxCurrentStrength(var1, var9, var3 - 1, var10, var6);
				}
			}

			if(var6 > 0) {
				--var6;
			} else {
				var6 = 0;
			}
		}

		if(var5 != var6) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, var6);
			var1->func_701_b(var2, var3, var4, var2, var3, var4);
			if(var6 > 0) {
				--var6;
			}

			for(var8 = 0; var8 < 4; ++var8) {
				var9 = var2;
				var10 = var4;
				int var11 = var3 - 1;
				if(var8 == 0) {
					var9 = var2 - 1;
				}

				if(var8 == 1) {
					++var9;
				}

				if(var8 == 2) {
					var10 = var4 - 1;
				}

				if(var8 == 3) {
					++var10;
				}

				if(var1->isBlockOpaqueCube(var9, var3, var10)) {
					var11 += 2;
				}

				int var12 = getMaxCurrentStrength(var1, var9, var3, var10, -1);
				if(var12 >= 0 && var12 != var6) {
					updateAndPropagateCurrentStrength(var1, var9, var3, var10);
				}

				var12 = getMaxCurrentStrength(var1, var9, var11, var10, -1);
				if(var12 >= 0 && var12 != var6) {
					updateAndPropagateCurrentStrength(var1, var9, var11, var10);
				}
			}

			if(var5 == 0 || var6 == 0) {
				var1->notifyBlocksOfNeighborChange(var2, var3, var4, blockID);
				var1->notifyBlocksOfNeighborChange(var2 - 1, var3, var4, blockID);
				var1->notifyBlocksOfNeighborChange(var2 + 1, var3, var4, blockID);
				var1->notifyBlocksOfNeighborChange(var2, var3, var4 - 1, blockID);
				var1->notifyBlocksOfNeighborChange(var2, var3, var4 + 1, blockID);
				var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, blockID);
				var1->notifyBlocksOfNeighborChange(var2, var3 + 1, var4, blockID);
			}
		}

}

void BlockRedstoneWire::notifyWireNeighborsOfNeighborChange(World* var1, int var2, int var3, int var4) {
		if(var1->getBlockId(var2, var3, var4) == blockID) {
			var1->notifyBlocksOfNeighborChange(var2, var3, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2 - 1, var3, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2 + 1, var3, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3, var4 - 1, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3, var4 + 1, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3 + 1, var4, blockID);
		}
}

void BlockRedstoneWire::onBlockAdded(World* var1, int var2, int var3, int var4) {
		Block::onBlockAdded(var1, var2, var3, var4);
		if(!var1->m_multiplayerWorld) {
			updateAndPropagateCurrentStrength(var1, var2, var3, var4);
			var1->notifyBlocksOfNeighborChange(var2, var3 + 1, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, blockID);
			notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3, var4);
			notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3, var4);
			notifyWireNeighborsOfNeighborChange(var1, var2, var3, var4 - 1);
			notifyWireNeighborsOfNeighborChange(var1, var2, var3, var4 + 1);
			if(var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
				notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3 + 1, var4);
			} else {
				notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3 - 1, var4);
			}

			if(var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
				notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3 + 1, var4);
			} else {
				notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3 - 1, var4);
			}

			if(var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
				notifyWireNeighborsOfNeighborChange(var1, var2, var3 + 1, var4 - 1);
			} else {
				notifyWireNeighborsOfNeighborChange(var1, var2, var3 - 1, var4 - 1);
			}

			if(var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
				notifyWireNeighborsOfNeighborChange(var1, var2, var3 + 1, var4 + 1);
			} else {
				notifyWireNeighborsOfNeighborChange(var1, var2, var3 - 1, var4 + 1);
			}

		}
}

void BlockRedstoneWire::onBlockRemoval(World* var1, int var2, int var3, int var4) {
		Block::onBlockRemoval(var1, var2, var3, var4);
		if(!var1->m_multiplayerWorld) {
			var1->notifyBlocksOfNeighborChange(var2, var3 + 1, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, blockID);
			updateAndPropagateCurrentStrength(var1, var2, var3, var4);
			notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3, var4);
			notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3, var4);
			notifyWireNeighborsOfNeighborChange(var1, var2, var3, var4 - 1);
			notifyWireNeighborsOfNeighborChange(var1, var2, var3, var4 + 1);
			if(var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
				notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3 + 1, var4);
			} else {
				notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3 - 1, var4);
			}

			if(var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
				notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3 + 1, var4);
			} else {
				notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3 - 1, var4);
			}

			if(var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
				notifyWireNeighborsOfNeighborChange(var1, var2, var3 + 1, var4 - 1);
			} else {
				notifyWireNeighborsOfNeighborChange(var1, var2, var3 - 1, var4 - 1);
			}

			if(var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
				notifyWireNeighborsOfNeighborChange(var1, var2, var3 + 1, var4 + 1);
			} else {
				notifyWireNeighborsOfNeighborChange(var1, var2, var3 - 1, var4 + 1);
			}

		}
}

int BlockRedstoneWire::getMaxCurrentStrength(World* var1, int var2, int var3, int var4, int var5) {
		if(var1->getBlockId(var2, var3, var4) != blockID) {
			return var5;
		} else {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			return var6 > var5 ? var6 : var5;
		}
}

void BlockRedstoneWire::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		if(!var1->m_multiplayerWorld) {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			bool var7 = canPlaceBlockAt(var1, var2, var3, var4);
			if(!var7) {
				dropBlockAsItem(var1, var2, var3, var4, var6);
				var1->setBlockWithNotify(var2, var3, var4, 0);
			} else {
				updateAndPropagateCurrentStrength(var1, var2, var3, var4);
			}

			Block::onNeighborBlockChange(var1, var2, var3, var4, var5);
		}
}

int BlockRedstoneWire::idDropped(int var1, Random* var2) {
		return Item::s_redstone->shiftedIndex;
}

bool BlockRedstoneWire::isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5) {
		return !m_wiresProvidePower ? false : isPoweringTo(var1, var2, var3, var4, var5);
}

bool BlockRedstoneWire::isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		if(!m_wiresProvidePower) {
			return false;
		} else if(var1->getBlockMetadata(var2, var3, var4) == 0) {
			return false;
		} else if(var5 == 1) {
			return true;
		} else {
			bool var6 = isPowerProviderOrWire(var1, var2 - 1, var3, var4) || !var1->isBlockOpaqueCube(var2 - 1, var3, var4) && isPowerProviderOrWire(var1, var2 - 1, var3 - 1, var4);
			bool var7 = isPowerProviderOrWire(var1, var2 + 1, var3, var4) || !var1->isBlockOpaqueCube(var2 + 1, var3, var4) && isPowerProviderOrWire(var1, var2 + 1, var3 - 1, var4);
			bool var8 = isPowerProviderOrWire(var1, var2, var3, var4 - 1) || !var1->isBlockOpaqueCube(var2, var3, var4 - 1) && isPowerProviderOrWire(var1, var2, var3 - 1, var4 - 1);
			bool var9 = isPowerProviderOrWire(var1, var2, var3, var4 + 1) || !var1->isBlockOpaqueCube(var2, var3, var4 + 1) && isPowerProviderOrWire(var1, var2, var3 - 1, var4 + 1);
			if(!var1->isBlockOpaqueCube(var2, var3 + 1, var4)) {
				if(var1->isBlockOpaqueCube(var2 - 1, var3, var4) && isPowerProviderOrWire(var1, var2 - 1, var3 + 1, var4)) {
					var6 = true;
				}

				if(var1->isBlockOpaqueCube(var2 + 1, var3, var4) && isPowerProviderOrWire(var1, var2 + 1, var3 + 1, var4)) {
					var7 = true;
				}

				if(var1->isBlockOpaqueCube(var2, var3, var4 - 1) && isPowerProviderOrWire(var1, var2, var3 + 1, var4 - 1)) {
					var8 = true;
				}

				if(var1->isBlockOpaqueCube(var2, var3, var4 + 1) && isPowerProviderOrWire(var1, var2, var3 + 1, var4 + 1)) {
					var9 = true;
				}
			}

			return !var8 && !var7 && !var6 && !var9 && var5 >= 2 && var5 <= 5 ? true : (var5 == 2 && var8 && !var6 && !var7 ? true : (var5 == 3 && var9 && !var6 && !var7 ? true : (var5 == 4 && var6 && !var8 && !var9 ? true : var5 == 5 && var7 && !var8 && !var9)));
		}
}

bool BlockRedstoneWire::canProvidePower() {
		return m_wiresProvidePower;
}

void BlockRedstoneWire::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(var1->getBlockMetadata(var2, var3, var4) > 0) {
			double var6 = (double)var2 + 0.5f + ((double)var5->nextFloat() - 0.5f) * 0.2f;
			double var8 = (double)((float)var3 + 1.0F / 16.0F);
			double var10 = (double)var4 + 0.5f + ((double)var5->nextFloat() - 0.5f) * 0.2f;
			var1->spawnParticle("reddust", var6, var8, var10, 0.0f, 0.0f, 0.0f);
		}

}

bool BlockRedstoneWire::isPowerProviderOrWire(IBlockAccess* var0, int var1, int var2, int var3) {
		int var4 = var0->getBlockId(var1, var2, var3);
		return var4 == Block::s_redstoneWire->blockID ? true : (var4 == 0 ? false : Block::blocksList[var4]->canProvidePower());
}
