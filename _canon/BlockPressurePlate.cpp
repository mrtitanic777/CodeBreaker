#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockPressurePlate.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockPressurePlate.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "EnumMobType.h"
#include "Explosion.h"
#include "File.h"
#include "IBlockAccess.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "InventoryPlayer.h"
#include "Item.h"
#include "ItemStack.h"
#include "Material.h"
#include "MobSpawnerBase.h"
#include "MovingObjectPosition.h"
#include "NBTBase.h"
#include "NBTTagCompound.h"
#include "NBTTagList.h"
#include "NibbleArray.h"
#include "NoiseGenerator2.h"
#include "NoiseGeneratorOctaves2.h"
#include "PathEntity.h"
#include "PathPoint.h"
#include "Random.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

BlockPressurePlate::BlockPressurePlate(int var1, int var2, int var3)
	: Block(var1, var2, Material::s_rock)
	, triggerMobType(0)
{
	this->triggerMobType = var3;
	this->setTickOnLoad(true);
	float var4 = 1.0F / 16.0F;
	this->setBlockBounds(var4, 0.0F, var4, 1.0F - var4, 0.03125F, 1.0F - var4);
}

BlockPressurePlate::~BlockPressurePlate()
{
}

int BlockPressurePlate::tickRate()
{
	return 20;
}

AxisAlignedBB* BlockPressurePlate::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return NULL;
}

bool BlockPressurePlate::isOpaqueCube()
{
	return false;
}

bool BlockPressurePlate::renderAsNormalBlock()
{
	return false;
}

bool BlockPressurePlate::canPlaceBlockAt(World* var1, int var2, int var3, int var4)
{
	return var1->isBlockOpaqueCube(var2, var3 - 1, var4);
}

void BlockPressurePlate::onBlockAdded(World* var1, int var2, int var3, int var4)
{
}

void BlockPressurePlate::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	bool var6 = false;
	if(!var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
	var6 = true;
	}
	if(var6) {
	this->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
	var1->setBlockWithNotify(var2, var3, var4, 0);
	}
}

void BlockPressurePlate::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(!var1->multiplayerWorld) {
	if(var1->getBlockMetadata(var2, var3, var4) != 0) {
	this->setStateIfMobInteractsWithPlate(var1, var2, var3, var4);
	}
	}
}

void BlockPressurePlate::onEntityCollidedWithBlock(World* var1, int var2, int var3, int var4, Entity* var5)
{
	if(!var1->multiplayerWorld) {
	if(var1->getBlockMetadata(var2, var3, var4) != 1) {
	this->setStateIfMobInteractsWithPlate(var1, var2, var3, var4);
	}
	}
}

void BlockPressurePlate::setStateIfMobInteractsWithPlate(World* var1, int var2, int var3, int var4)
{
	bool var5 = var1->getBlockMetadata(var2, var3, var4) == 1;
	bool var6 = false;
	float var7 = 2.0F / 16.0F;
	JList<void*> var8 = NULL;
	if(this->triggerMobType == EnumMobType::everything) {
	var8 = var1->getEntitiesWithinAABBExcludingEntity((Entity*)NULL, AxisAlignedBB::getBoundingBoxFromPool((double)((float)var2 + var7), (double)var3, (double)((float)var4 + var7), (double)((float)(var2 + 1) - var7), (double)var3 + 0.25, (double)((float)(var4 + 1) - var7)));
	}
	if(this->triggerMobType == EnumMobType::mobs) {
	var8 = var1->getEntitiesWithinAABB(Class(typeid(EntityLiving)), AxisAlignedBB::getBoundingBoxFromPool((double)((float)var2 + var7), (double)var3, (double)((float)var4 + var7), (double)((float)(var2 + 1) - var7), (double)var3 + 0.25, (double)((float)(var4 + 1) - var7)));
	}
	if(this->triggerMobType == EnumMobType::players) {
	var8 = var1->getEntitiesWithinAABB(Class(typeid(EntityPlayer)), AxisAlignedBB::getBoundingBoxFromPool((double)((float)var2 + var7), (double)var3, (double)((float)var4 + var7), (double)((float)(var2 + 1) - var7), (double)var3 + 0.25, (double)((float)(var4 + 1) - var7)));
	}
	if(var8.size() > 0) {
	var6 = true;
	}
	if(var6 && !var5) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, 1);
	var1->notifyBlocksOfNeighborChange(var2, var3, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, this->blockID);
	var1->markBlockRangeNeedsUpdate(var2, var3, var4, var2, var3, var4);
	var1->playSoundEffect((double)var2 + 0.5,(double)var3 + 0.1,(double)var4 + 0.5,"random.click",0.3F,0.6F);
	}
	if(!var6 && var5) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, 0);
	var1->notifyBlocksOfNeighborChange(var2, var3, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, this->blockID);
	var1->markBlockRangeNeedsUpdate(var2, var3, var4, var2, var3, var4);
	var1->playSoundEffect((double)var2 + 0.5,(double)var3 + 0.1,(double)var4 + 0.5,"random.click",0.3F,0.5F);
	}
	if(var6) {
	var1->scheduleBlockUpdate(var2, var3, var4, this->blockID);
	}
}

void BlockPressurePlate::onBlockRemoval(World* var1, int var2, int var3, int var4)
{
	int var5 = var1->getBlockMetadata(var2, var3, var4);
	if(var5 > 0) {
	var1->notifyBlocksOfNeighborChange(var2, var3, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, this->blockID);
	}
	Block::onBlockRemoval(var1, var2, var3, var4);
}

void BlockPressurePlate::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4)
{
	bool var5 = var1->getBlockMetadata(var2, var3, var4) == 1;
	float var6 = 1.0F / 16.0F;
	if(var5) {
	this->setBlockBounds(var6, 0.0F, var6, 1.0F - var6, 0.03125F, 1.0F - var6);
	} else {
	this->setBlockBounds(var6, 0.0F, var6, 1.0F - var6, 1.0F / 16.0F, 1.0F - var6);
	}
}

bool BlockPressurePlate::isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	return var1->getBlockMetadata(var2, var3, var4) > 0;
}

bool BlockPressurePlate::isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5)
{
	return var1->getBlockMetadata(var2, var3, var4) == 0 ? false : var5 == 1;
}

bool BlockPressurePlate::canProvidePower()
{
	return true;
}

void BlockPressurePlate::setBlockBoundsForItemRender()
{
	float var1 = 0.5F;
	float var2 = 2.0F / 16.0F;
	float var3 = 0.5F;
	this->setBlockBounds(0.5F - var1, 0.5F - var2, 0.5F - var3, 0.5F + var1, 0.5F + var2, 0.5F + var3);
}

void BlockPressurePlate::initStatics()
{
}
