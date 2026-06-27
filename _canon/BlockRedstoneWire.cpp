#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockRedstoneWire.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockRedstoneWire.h"
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

BlockRedstoneWire::BlockRedstoneWire(int var1, int var2)
	: Block(var1, var2, Material::s_circuits)
	, wiresProvidePower(0)
{
	this->wiresProvidePower = true;
	this->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F / 16.0F, 1.0F);
}

BlockRedstoneWire::~BlockRedstoneWire()
{
}

int BlockRedstoneWire::getBlockTextureFromSideAndMetadata(int var1, int var2)
{
	return this->blockIndexInTexture + (var2 > 0 ? 16 : 0);
}

AxisAlignedBB* BlockRedstoneWire::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return NULL;
}

bool BlockRedstoneWire::isOpaqueCube()
{
	return false;
}

bool BlockRedstoneWire::renderAsNormalBlock()
{
	return false;
}

int BlockRedstoneWire::getRenderType()
{
	return 5;
}

bool BlockRedstoneWire::canPlaceBlockAt(World* var1, int var2, int var3, int var4)
{
	return var1->isBlockOpaqueCube(var2, var3 - 1, var4);
}

void BlockRedstoneWire::updateAndPropagateCurrentStrength(World* var1, int var2, int var3, int var4)
{
	int var5 = var1->getBlockMetadata(var2, var3, var4);
	int var6 = 0;
	this->wiresProvidePower = false;
	bool var7 = var1->isBlockIndirectlyGettingPowered(var2, var3, var4);
	this->wiresProvidePower = true;
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
	var6 = this->getMaxCurrentStrength(var1, var9, var3, var10, var6);
	if(var1->isBlockOpaqueCube(var9, var3, var10) && !var1->isBlockOpaqueCube(var2, var3 + 1, var4)) {
	var6 = this->getMaxCurrentStrength(var1, var9, var3 + 1, var10, var6);
	} else if(!var1->isBlockOpaqueCube(var9, var3, var10)) {
	var6 = this->getMaxCurrentStrength(var1, var9, var3 - 1, var10, var6);
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
	var1->markBlockRangeNeedsUpdate(var2, var3, var4, var2, var3, var4);
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
	int var12 = this->getMaxCurrentStrength(var1, var9, var3, var10, -1);
	if(var12 >= 0 && var12 != var6) {
	this->updateAndPropagateCurrentStrength(var1, var9, var3, var10);
	}
	var12 = this->getMaxCurrentStrength(var1, var9, var11, var10, -1);
	if(var12 >= 0 && var12 != var6) {
	this->updateAndPropagateCurrentStrength(var1, var9, var11, var10);
	}
	}
	if(var5 == 0 || var6 == 0) {
	var1->notifyBlocksOfNeighborChange(var2, var3, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2 - 1, var3, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2 + 1, var3, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3, var4 - 1, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3, var4 + 1, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3 + 1, var4, this->blockID);
	}
	}
}

void BlockRedstoneWire::notifyWireNeighborsOfNeighborChange(World* var1, int var2, int var3, int var4)
{
	if(var1->getBlockId(var2, var3, var4) == this->blockID) {
	var1->notifyBlocksOfNeighborChange(var2, var3, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2 - 1, var3, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2 + 1, var3, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3, var4 - 1, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3, var4 + 1, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3 + 1, var4, this->blockID);
	}
}

void BlockRedstoneWire::onBlockAdded(World* var1, int var2, int var3, int var4)
{
	Block::onBlockAdded(var1, var2, var3, var4);
	if(!var1->multiplayerWorld) {
	this->updateAndPropagateCurrentStrength(var1, var2, var3, var4);
	var1->notifyBlocksOfNeighborChange(var2, var3 + 1, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, this->blockID);
	this->notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3, var4);
	this->notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3, var4);
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3, var4 - 1);
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3, var4 + 1);
	if(var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
	this->notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3 + 1, var4);
	} else {
	this->notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3 - 1, var4);
	}
	if(var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
	this->notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3 + 1, var4);
	} else {
	this->notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3 - 1, var4);
	}
	if(var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3 + 1, var4 - 1);
	} else {
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3 - 1, var4 - 1);
	}
	if(var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3 + 1, var4 + 1);
	} else {
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3 - 1, var4 + 1);
	}
	}
}

void BlockRedstoneWire::onBlockRemoval(World* var1, int var2, int var3, int var4)
{
	Block::onBlockRemoval(var1, var2, var3, var4);
	if(!var1->multiplayerWorld) {
	var1->notifyBlocksOfNeighborChange(var2, var3 + 1, var4, this->blockID);
	var1->notifyBlocksOfNeighborChange(var2, var3 - 1, var4, this->blockID);
	this->updateAndPropagateCurrentStrength(var1, var2, var3, var4);
	this->notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3, var4);
	this->notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3, var4);
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3, var4 - 1);
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3, var4 + 1);
	if(var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
	this->notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3 + 1, var4);
	} else {
	this->notifyWireNeighborsOfNeighborChange(var1, var2 - 1, var3 - 1, var4);
	}
	if(var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
	this->notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3 + 1, var4);
	} else {
	this->notifyWireNeighborsOfNeighborChange(var1, var2 + 1, var3 - 1, var4);
	}
	if(var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3 + 1, var4 - 1);
	} else {
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3 - 1, var4 - 1);
	}
	if(var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3 + 1, var4 + 1);
	} else {
	this->notifyWireNeighborsOfNeighborChange(var1, var2, var3 - 1, var4 + 1);
	}
	}
}

int BlockRedstoneWire::getMaxCurrentStrength(World* var1, int var2, int var3, int var4, int var5)
{
	if(var1->getBlockId(var2, var3, var4) != this->blockID) {
	return var5;
	} else {
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	return var6 > var5 ? var6 : var5;
	}
}

void BlockRedstoneWire::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	if(!var1->multiplayerWorld) {
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	bool var7 = this->canPlaceBlockAt(var1, var2, var3, var4);
	if(!var7) {
	this->dropBlockAsItem(var1, var2, var3, var4, var6);
	var1->setBlockWithNotify(var2, var3, var4, 0);
	} else {
	this->updateAndPropagateCurrentStrength(var1, var2, var3, var4);
	}
	Block::onNeighborBlockChange(var1, var2, var3, var4, var5);
	}
}

int BlockRedstoneWire::idDropped(int var1, Random* var2)
{
	return Item::s_redstone->shiftedIndex;
}

bool BlockRedstoneWire::isIndirectlyPoweringTo(World* var1, int var2, int var3, int var4, int var5)
{
	return !this->wiresProvidePower ? false : this->isPoweringTo(var1, var2, var3, var4, var5);
}

bool BlockRedstoneWire::isPoweringTo(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	if(!this->wiresProvidePower) {
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

bool BlockRedstoneWire::canProvidePower()
{
	return this->wiresProvidePower;
}

void BlockRedstoneWire::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(var1->getBlockMetadata(var2, var3, var4) > 0) {
	double var6 = (double)var2 + 0.5 + ((double)var5->nextFloat() - 0.5) * 0.2;
	double var8 = (double)((float)var3 + 1.0F / 16.0F);
	double var10 = (double)var4 + 0.5 + ((double)var5->nextFloat() - 0.5) * 0.2;
	var1->spawnParticle("reddust", var6, var8, var10, 0.0, 0.0, 0.0);
	}
}

bool BlockRedstoneWire::isPowerProviderOrWire(IBlockAccess* var0, int var1, int var2, int var3)
{
	int var4 = var0->getBlockId(var1, var2, var3);
	return var4 == Block::s_redstoneWire->blockID ? true : (var4 == 0 ? false : Block::s_blocksList[var4]->canProvidePower());
}

void BlockRedstoneWire::initStatics()
{
}
