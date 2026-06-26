#include "stdafx.h"
// Auto-generated from BlockRedstoneTorch.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockRedstoneTorch.h"
#include "Common.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "World.h"
#include <algorithm>
#include "Block.h"
#include "BlockTorch.h"
#include "RedstoneUpdateInfo.h"

std::vector<void*> /* PORT: determine element type */ BlockRedstoneTorch::s_torchUpdates;

BlockRedstoneTorch::BlockRedstoneTorch()
	: m_torchActive(false)
{
}

BlockRedstoneTorch::BlockRedstoneTorch(int var1, int var2, bool var3)
	: BlockTorch(var1, var2)
	, m_torchActive(false)
{
		m_torchActive = var3;
		setTickOnLoad(true);
}

BlockRedstoneTorch::~BlockRedstoneTorch() {
	// No dynamic resources
}

int BlockRedstoneTorch::getBlockTextureFromSideAndMetadata(int var1, int var2) {
		return var1 == 1 ? Block::s_redstoneWire->getBlockTextureFromSideAndMetadata(var1, var2) : BlockTorch::getBlockTextureFromSideAndMetadata(var1, var2);
}

bool BlockRedstoneTorch::checkForBurnout(World* var1, int var2, int var3, int var4, bool var5) {
		if(var5) {
			s_torchUpdates.push_back(new RedstoneUpdateInfo(var2, var3, var4, var1->m_worldTime));
		}

		int var6 = 0;

		for(size_t var7 = 0; var7 < s_torchUpdates.size(); ++var7) {
			RedstoneUpdateInfo* var8 = (RedstoneUpdateInfo*)s_torchUpdates[var7];
			if(var8->m_x == var2 && var8->m_y == var3 && var8->m_z == var4) {
				++var6;
				if(var6 >= 8) {
					return true;
				}
			}
		}

		return false;
}

int BlockRedstoneTorch::tickRate() {
		return 2;
}

void BlockRedstoneTorch::onBlockAdded(World* var1, int var2, int var3, int var4) {
		if(var1->getBlockMetadata(var2, var3, var4) == 0) {
			BlockTorch::onBlockAdded(var1, var2, var3, var4);
		}

		if(m_torchActive) {
			var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3 + 1, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2 - 1, var3, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2 + 1, var3, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3, var4 - 1, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3, var4 + 1, blockID);
		}

}

void BlockRedstoneTorch::onBlockRemoval(World* var1, int var2, int var3, int var4) {
		if(m_torchActive) {
			var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3 + 1, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2 - 1, var3, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2 + 1, var3, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3, var4 - 1, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3, var4 + 1, blockID);
		}

}

bool BlockRedstoneTorch::isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		if(!m_torchActive) {
			return false;
		} else {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			return var6 == 5 && var5 == 1 ? false : (var6 == 3 && var5 == 3 ? false : (var6 == 4 && var5 == 2 ? false : (var6 == 1 && var5 == 5 ? false : var6 != 2 || var5 != 4)));
		}
}

bool BlockRedstoneTorch::func_15232_h(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4);
		return var5 == 5 && var1->isBlockIndirectlyProvidingPowerTo(var2, var3 - 1, var4, 0) ? true : (var5 == 3 && var1->isBlockIndirectlyProvidingPowerTo(var2, var3, var4 - 1, 2) ? true : (var5 == 4 && var1->isBlockIndirectlyProvidingPowerTo(var2, var3, var4 + 1, 3) ? true : (var5 == 1 && var1->isBlockIndirectlyProvidingPowerTo(var2 - 1, var3, var4, 4) ? true : var5 == 2 && var1->isBlockIndirectlyProvidingPowerTo(var2 + 1, var3, var4, 5))));
}

void BlockRedstoneTorch::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		bool var6 = func_15232_h(var1, var2, var3, var4);

		while(s_torchUpdates.size() > 0 && var1->m_worldTime - ((RedstoneUpdateInfo*)s_torchUpdates[0])->m_updateTime > 100L) {
   // PORT: Java did torchUpdates.remove(0) -- pop & free the expired front entry
   delete (RedstoneUpdateInfo*)s_torchUpdates[0];
   s_torchUpdates.erase(s_torchUpdates.begin());
		}

		if(m_torchActive) {
			if(var6) {
				var1->setBlockAndMetadataWithNotify(var2, var3, var4, Block::s_torchRedstoneIdle->blockID, var1->getBlockMetadata(var2, var3, var4));
				if(checkForBurnout(var1, var2, var3, var4, true)) {
					var1->playSoundEffect((double)((float)var2 + 0.5F), (double)((float)var3 + 0.5F), (double)((float)var4 + 0.5F), "random.fizz", 0.5F, 2.6F + (var1->rand->nextFloat() - var1->rand->nextFloat()) * 0.8F);

					for(int var7 = 0; var7 < 5; ++var7) {
						double var8 = (double)var2 + var5->nextDouble() * 0.6f + 0.2f;
						double var10 = (double)var3 + var5->nextDouble() * 0.6f + 0.2f;
						double var12 = (double)var4 + var5->nextDouble() * 0.6f + 0.2f;
						var1->spawnParticle("smoke", var8, var10, var12, 0.0f, 0.0f, 0.0f);
					}
				}
			}
		} else if(!var6 && !checkForBurnout(var1, var2, var3, var4, false)) {
			var1->setBlockAndMetadataWithNotify(var2, var3, var4, Block::s_torchRedstoneActive->blockID, var1->getBlockMetadata(var2, var3, var4));
		}

}

void BlockRedstoneTorch::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		BlockTorch::onNeighborBlockChange(var1, var2, var3, var4, var5);
		var1->scheduleBlockUpdate(var2, var3, var4, blockID);
}

bool BlockRedstoneTorch::isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5) {
		return var5 == 0 ? isPoweringTo(var1, var2, var3, var4, var5) : false;
}

int BlockRedstoneTorch::idDropped(int var1, Random* var2) {
		return Block::s_torchRedstoneActive->blockID;
}

bool BlockRedstoneTorch::canProvidePower() {
		return true;
}

void BlockRedstoneTorch::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(m_torchActive) {
			int var6 = var1->getBlockMetadata(var2, var3, var4);
			double var7 = (double)((float)var2 + 0.5F) + (double)(var5->nextFloat() - 0.5F) * 0.2f;
			double var9 = (double)((float)var3 + 0.7F) + (double)(var5->nextFloat() - 0.5F) * 0.2f;
			double var11 = (double)((float)var4 + 0.5F) + (double)(var5->nextFloat() - 0.5F) * 0.2f;
			double var13 = (double)0.22F;
			double var15 = (double)0.27F;
			if(var6 == 1) {
				var1->spawnParticle("reddust", var7 - var15, var9 + var13, var11, 0.0f, 0.0f, 0.0f);
			} else if(var6 == 2) {
				var1->spawnParticle("reddust", var7 + var15, var9 + var13, var11, 0.0f, 0.0f, 0.0f);
			} else if(var6 == 3) {
				var1->spawnParticle("reddust", var7, var9 + var13, var11 - var15, 0.0f, 0.0f, 0.0f);
			} else if(var6 == 4) {
				var1->spawnParticle("reddust", var7, var9 + var13, var11 + var15, 0.0f, 0.0f, 0.0f);
			} else {
				var1->spawnParticle("reddust", var7, var9, var11, 0.0f, 0.0f, 0.0f);
			}

		}
}
