#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockButton.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockButton.h"
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
#include "EntityPlayer.h"
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

BlockButton::BlockButton(int var1, int var2)
	: Block(var1, var2, Material::s_circuits)
{
	this->setTickOnLoad(true);
}

BlockButton::~BlockButton()
{
}

AxisAlignedBB* BlockButton::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return NULL;
}

int BlockButton::tickRate()
{
	return 20;
}

bool BlockButton::isOpaqueCube()
{
	return false;
}

bool BlockButton::renderAsNormalBlock()
{
	return false;
}

bool BlockButton::canPlaceBlockAt(World* var1, int var2, int var3, int var4)
{
	return var1->isBlockOpaqueCube(var2 - 1, var3, var4) ? true : (var1->isBlockOpaqueCube(var2 + 1, var3, var4) ? true : (var1->isBlockOpaqueCube(var2, var3, var4 - 1) ? true : var1->isBlockOpaqueCube(var2, var3, var4 + 1)));
}

void BlockButton::onBlockPlaced(World* var1, int var2, int var3, int var4, int var5)
{
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

void BlockButton::onBlockAdded(World* var1, int var2, int var3, int var4)
{
	if(var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, 1);
	} else if(var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, 2);
	} else if(var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, 3);
	} else if(var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, 4);
	}
	this->checkIfAttachedValid(var1, var2, var3, var4);
}

void BlockButton::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	if(this->checkIfAttachedValid(var1, var2, var3, var4)) {
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
	this->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
	var1->setBlockWithNotify(var2, var3, var4, 0);
	}
	}
}

bool BlockButton::checkIfAttachedValid(World* var1, int var2, int var3, int var4)
{
	if(!this->canPlaceBlockAt(var1, var2, var3, var4)) {
	this->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
	var1->setBlockWithNotify(var2, var3, var4, 0);
	return false;
	} else {
	return true;
	}
}

void BlockButton::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4)
{
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
	this->setBlockBounds(0.0F, var8, 0.5F - var10, var11, var9, 0.5F + var10);
	} else if(var6 == 2) {
	this->setBlockBounds(1.0F - var11, var8, 0.5F - var10, 1.0F, var9, 0.5F + var10);
	} else if(var6 == 3) {
	this->setBlockBounds(0.5F - var10, var8, 0.0F, 0.5F + var10, var9, var11);
	} else if(var6 == 4) {
	this->setBlockBounds(0.5F - var10, var8, 1.0F - var11, 0.5F + var10, var9, 1.0F);
	}
}

void BlockButton::onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5)
{
	this->blockActivated(var1, var2, var3, var4, var5);
}

bool BlockButton::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5)
{
	if(var1->multiplayerWorld) {
	return true;
	} else {
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	int var7 = var6 & 7;
	int var8 = 8 - (var6 & 8);
	if(var8 == 0) {
	return true;
	} else {
	var1->setBlockMetadataWithNotify(var2, var3, var4, var7 + var8);
	var1->markBlockRangeNeedsUpdate(var2, var3, var4, var2, var3, var4);
	var1->playSoundEffect((double)var2 + 0.5,(double)var3 + 0.5,(double)var4 + 0.5,"random.click",0.3F,0.6F);
	var1->notifyBlocksOfNeighborChange(var2, var3, var4, this->blockID);
	if(var7 == 1) {
	var1->notifyBlocksOfNeighborChange(var2 - 1, var3, var4, this->blockID);
	} else if(var7 == 2) {
	var1->notifyBlocksOfNeighborChange(var2 + 1, var3, var4, this->blockID);
	} else if(var7 == 3) {
	var1->notifyBlocksOfNeighborChange(var2, var3, var4 - 1, this->blockID);
	} else if(var7 == 4) {
	var1->notifyBlocksOfNeighborChange(var2, var3, var4 + 1, this->blockID);
	} else {
	var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, this->blockID);
	}
	var1->scheduleBlockUpdate(var2, var3, var4, this->blockID);
	return true;
	}
	}
}

void BlockButton::onBlockRemoval(World* var1, int var2, int var3, int var4)
{
	int var5 = var1->getBlockMetadata(var2, var3, var4);
	if((var5 & 8) > 0) {
	var1->notifyBlocksOfNeighborChange(var2, var3, var4, this->blockID);
	int var6 = var5 & 7;
	if(var6 == 1) {
	var1->notifyBlocksOfNeighborChange(var2 - 1, var3, var4, this->blockID);
	} else if(var6 == 2) {
	var1->notifyBlocksOfNeighborChange(var2 + 1, var3, var4, this->blockID);
	} else if(var6 == 3) {
	var1->notifyBlocksOfNeighborChange(var2, var3, var4 - 1, this->blockID);
	} else if(var6 == 4) {
	var1->notifyBlocksOfNeighborChange(var2, var3, var4 + 1, this->blockID);
	} else {
	var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, this->blockID);
	}
	}
	Block::onBlockRemoval(var1, var2, var3, var4);
}

bool BlockButton::isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	return (var1->getBlockMetadata(var2, var3, var4) & 8) > 0;
}

bool BlockButton::isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5)
{
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	if((var6 & 8) == 0) {
	return false;
	} else {
	int var7 = var6 & 7;
	return var7 == 5 && var5 == 1 ? true : (var7 == 4 && var5 == 2 ? true : (var7 == 3 && var5 == 3 ? true : (var7 == 2 && var5 == 4 ? true : var7 == 1 && var5 == 5)));
	}
}

bool BlockButton::canProvidePower()
{
	return true;
}

void BlockButton::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(!var1->multiplayerWorld) {
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	if((var6 & 8) != 0) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, var6 & 7);
	var1->notifyBlocksOfNeighborChange(var2, var3, var4, this->blockID);
	int var7 = var6 & 7;
	if(var7 == 1) {
	var1->notifyBlocksOfNeighborChange(var2 - 1, var3, var4, this->blockID);
	} else if(var7 == 2) {
	var1->notifyBlocksOfNeighborChange(var2 + 1, var3, var4, this->blockID);
	} else if(var7 == 3) {
	var1->notifyBlocksOfNeighborChange(var2, var3, var4 - 1, this->blockID);
	} else if(var7 == 4) {
	var1->notifyBlocksOfNeighborChange(var2, var3, var4 + 1, this->blockID);
	} else {
	var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, this->blockID);
	}
	var1->playSoundEffect((double)var2 + 0.5,(double)var3 + 0.5,(double)var4 + 0.5,"random.click",0.3F,0.5F);
	var1->markBlockRangeNeedsUpdate(var2, var3, var4, var2, var3, var4);
	}
	}
}

void BlockButton::setBlockBoundsForItemRender()
{
	float var1 = 3.0F / 16.0F;
	float var2 = 2.0F / 16.0F;
	float var3 = 2.0F / 16.0F;
	this->setBlockBounds(0.5F - var1, 0.5F - var2, 0.5F - var3, 0.5F + var1, 0.5F + var2, 0.5F + var3);
}

void BlockButton::initStatics()
{
}
