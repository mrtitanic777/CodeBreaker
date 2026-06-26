#include "stdafx.h"
// Auto-generated from BlockPressurePlate.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockPressurePlate.h"
#include "Common.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "EnumMobType.h"
#include "IBlockAccess.h"
#include "Random.h"
#include "World.h"
#include "Block.h"
#include "Material.h"

BlockPressurePlate::BlockPressurePlate()
	: m_triggerMobType(NULL)
{
}

BlockPressurePlate::BlockPressurePlate(int var1, int var2, EnumMobType* var3)
	: Block(var1, var2, Material::s_rock)
	, m_triggerMobType(NULL)
{
		m_triggerMobType = var3;
		setTickOnLoad(true);
		float var4 = 1.0F / 16.0F;
		setBlockBounds(var4, 0.0F, var4, 1.0F - var4, 0.03125F, 1.0F - var4);
}

BlockPressurePlate::~BlockPressurePlate() {
	m_triggerMobType = NULL;
}

int BlockPressurePlate::tickRate() {
		return 20;
}

AxisAlignedBB BlockPressurePlate::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4) {
		return AxisAlignedBB();
}

bool BlockPressurePlate::isOpaqueCube() {
		return false;
}

bool BlockPressurePlate::renderAsNormalBlock() {
		return false;
}

bool BlockPressurePlate::canPlaceBlockAt(World* var1, int var2, int var3, int var4) {
		return var1->isBlockOpaqueCube(var2, var3 - 1, var4);
}

void BlockPressurePlate::onBlockAdded(World* var1, int var2, int var3, int var4) {
	// TODO: implement
}

void BlockPressurePlate::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5) {
		bool var6 = false;
		if(!var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
			var6 = true;
		}

		if(var6) {
			dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
			var1->setBlockWithNotify(var2, var3, var4, 0);
		}

}

void BlockPressurePlate::updateTick(World* var1, int var2, int var3, int var4, Random* var5) {
		if(!var1->m_multiplayerWorld) {
			if(var1->getBlockMetadata(var2, var3, var4) != 0) {
				setStateIfMobInteractsWithPlate(var1, var2, var3, var4);
			}
		}
}

void BlockPressurePlate::onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5) {
		if(!var1->m_multiplayerWorld) {
			if(var1->getBlockMetadata(var2, var3, var4) != 1) {
				setStateIfMobInteractsWithPlate(var1, var2, var3, var4);
			}
		}
}

void BlockPressurePlate::setStateIfMobInteractsWithPlate(World* var1, int var2, int var3, int var4) {
		bool var5 = var1->getBlockMetadata(var2, var3, var4) == 1;
		bool var6 = false;
		float var7 = 2.0F / 16.0F;
		std::vector<void*> var8;  // PORT: was '= NULL' (illegal vector init); default-construct empty
		AxisAlignedBB var9 = AxisAlignedBB::getBoundingBoxFromPool((double)((float)var2 + var7), (double)var3, (double)((float)var4 + var7), (double)((float)(var2 + 1) - var7), (double)var3 + 0.25f, (double)((float)(var4 + 1) - var7));
		if(m_triggerMobType == EnumMobType::s_everything) {
			var8 = var1->getEntitiesWithinAABBExcludingEntity(NULL, var9);
		}

		// PORT: Java filtered by EntityLiving.class / EntityPlayer.class. No RTTI class
		// token on Xbox 360 (the World/Chunk type filter is a no-op in this port), so
		// these collect all nearby entities like the "everything" case.
		if(m_triggerMobType == EnumMobType::s_mobs) {
			var8 = var1->getEntitiesWithinAABBExcludingEntity(NULL, var9);
		}

		if(m_triggerMobType == EnumMobType::s_players) {
			var8 = var1->getEntitiesWithinAABBExcludingEntity(NULL, var9);
		}

		if(var8.size() > 0) {
			var6 = true;
		}

		if(var6 && !var5) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 1);
			var1->notifyBlocksOfNeighborChange(var2, var3, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, blockID);
			var1->func_701_b(var2, var3, var4, var2, var3, var4);
			var1->playSoundEffect((double)var2 + 0.5f, (double)var3 + 0.1f, (double)var4 + 0.5f, "random.click", 0.3F, 0.6F);
		}

		if(!var6 && var5) {
			var1->setBlockMetadataWithNotify(var2, var3, var4, 0);
			var1->notifyBlocksOfNeighborChange(var2, var3, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, blockID);
			var1->func_701_b(var2, var3, var4, var2, var3, var4);
			var1->playSoundEffect((double)var2 + 0.5f, (double)var3 + 0.1f, (double)var4 + 0.5f, "random.click", 0.3F, 0.5F);
		}

		if(var6) {
			var1->scheduleBlockUpdate(var2, var3, var4, blockID);
		}

}

void BlockPressurePlate::onBlockRemoval(World* var1, int var2, int var3, int var4) {
		int var5 = var1->getBlockMetadata(var2, var3, var4);
		if(var5 > 0) {
			var1->notifyBlocksOfNeighborChange(var2, var3, var4, blockID);
			var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, blockID);
		}

		Block::onBlockRemoval(var1, var2, var3, var4);
}

void BlockPressurePlate::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4) {
		bool var5 = var1->getBlockMetadata(var2, var3, var4) == 1;
		float var6 = 1.0F / 16.0F;
		if(var5) {
			setBlockBounds(var6, 0.0F, var6, 1.0F - var6, 0.03125F, 1.0F - var6);
		} else {
			setBlockBounds(var6, 0.0F, var6, 1.0F - var6, 1.0F / 16.0F, 1.0F - var6);
		}

}

bool BlockPressurePlate::isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5) {
		return var1->getBlockMetadata(var2, var3, var4) > 0;
}

bool BlockPressurePlate::isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5) {
		return var1->getBlockMetadata(var2, var3, var4) == 0 ? false : var5 == 1;
}

bool BlockPressurePlate::canProvidePower() {
		return true;
}

void BlockPressurePlate::func_237_e() {
		float var1 = 0.5F;
		float var2 = 2.0F / 16.0F;
		float var3 = 0.5F;
		setBlockBounds(0.5F - var1, 0.5F - var2, 0.5F - var3, 0.5F + var1, 0.5F + var2, 0.5F + var3);
}
