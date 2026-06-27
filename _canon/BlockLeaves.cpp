#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockLeaves.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockLeaves.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeavesBase.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "Class.h"
#include "ColorizerFoliage.h"
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

BlockLeaves::BlockLeaves(int var1, int var2)
	: BlockLeavesBase(var1, var2, Material::s_leaves, false)
	, baseIndexInPNG(0)
	, leafDecayCounter(0)
{
	this->leafDecayCounter = 0;
	this->baseIndexInPNG = var2;
}

BlockLeaves::~BlockLeaves()
{
}

int BlockLeaves::colorMultiplier(IBlockAccess* var1, int var2, int var3, int var4)
{
	var1->getWorldChunkManager()->getBiomesGrid(var2, var4, 1, 1);
	double var5 = var1->getWorldChunkManager()->temperature[0];
	double var7 = var1->getWorldChunkManager()->humidity[0];
	return ColorizerFoliage::getFoliageColor(var5, var7);
}

void BlockLeaves::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	if(this == NULL) {
	this->leafDecayCounter = 0;
	this->updateLeafDecay(var1, var2, var3, var4);
	BlockLeavesBase::onNeighborBlockChange(var1, var2, var3, var4, var5);
	}
}

void BlockLeaves::propagateLeafDecay(World* var1, int var2, int var3, int var4, int var5)
{
	if(var1->getBlockId(var2, var3, var4) == this->blockID) {
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	if(var6 != 0 && var6 == var5 - 1) {
	this->updateLeafDecay(var1, var2, var3, var4);
	}
	}
}

void BlockLeaves::updateLeafDecay(World* var1, int var2, int var3, int var4)
{
	if(this == NULL) {
	if(this->leafDecayCounter++ < 100) {
	int var5 = var1->getBlockMaterial(var2, var3 - 1, var4)->isSolid() ? 16 : 0;
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	if(var6 == 0) {
	var6 = 1;
	var1->setBlockMetadataWithNotify(var2, var3, var4, 1);
	}
	var5 = this->getMaxNeighborDecayValue(var1, var2, var3 - 1, var4, var5);
	var5 = this->getMaxNeighborDecayValue(var1, var2, var3, var4 - 1, var5);
	var5 = this->getMaxNeighborDecayValue(var1, var2, var3, var4 + 1, var5);
	var5 = this->getMaxNeighborDecayValue(var1, var2 - 1, var3, var4, var5);
	var5 = this->getMaxNeighborDecayValue(var1, var2 + 1, var3, var4, var5);
	int var7 = var5 - 1;
	if(var7 < 10) {
	var7 = 1;
	}
	if(var7 != var6) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, var7);
	this->propagateLeafDecay(var1, var2, var3 - 1, var4, var6);
	this->propagateLeafDecay(var1, var2, var3 + 1, var4, var6);
	this->propagateLeafDecay(var1, var2, var3, var4 - 1, var6);
	this->propagateLeafDecay(var1, var2, var3, var4 + 1, var6);
	this->propagateLeafDecay(var1, var2 - 1, var3, var4, var6);
	this->propagateLeafDecay(var1, var2 + 1, var3, var4, var6);
	}
	}
	}
}

int BlockLeaves::getMaxNeighborDecayValue(World* var1, int var2, int var3, int var4, int var5)
{
	int var6 = var1->getBlockId(var2, var3, var4);
	if(var6 == Block::s_wood->blockID) {
	return 16;
	} else {
	if(var6 == this->blockID) {
	int var7 = var1->getBlockMetadata(var2, var3, var4);
	if(var7 != 0 && var7 > var5) {
	return var7;
	}
	}
	return var5;
	}
}

void BlockLeaves::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(this == NULL) {
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	if(var6 == 0) {
	this->leafDecayCounter = 0;
	this->updateLeafDecay(var1, var2, var3, var4);
	} else if(var6 == 1) {
	this->removeLeafBlock(var1, var2, var3, var4);
	} else if(var5->nextInt(10) == 0) {
	this->updateLeafDecay(var1, var2, var3, var4);
	}
	}
}

void BlockLeaves::removeLeafBlock(World* var1, int var2, int var3, int var4)
{
	this->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
	var1->setBlockWithNotify(var2, var3, var4, 0);
}

int BlockLeaves::quantityDropped(Random* var1)
{
	return var1->nextInt(20) == 0 ? 1 : 0;
}

int BlockLeaves::idDropped(int var1, Random* var2)
{
	return Block::s_sapling->blockID;
}

bool BlockLeaves::isOpaqueCube()
{
	return !this->graphicsLevel;
}

void BlockLeaves::setGraphicsLevel(bool var1)
{
	this->graphicsLevel = var1;
	this->blockIndexInTexture = this->baseIndexInPNG + (var1 ? 0 : 1);
}

void BlockLeaves::onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5)
{
	BlockLeavesBase::onEntityWalking(var1, var2, var3, var4, var5);
}

void BlockLeaves::initStatics()
{
}
