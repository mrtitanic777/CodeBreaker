#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockSnow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSnow.h"
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
#include "EnumSkyBlock.h"
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

BlockSnow::BlockSnow(int var1, int var2)
	: Block(var1, var2, Material::s_snow)
{
	this->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 2.0F / 16.0F, 1.0F);
	this->setTickOnLoad(true);
}

BlockSnow::~BlockSnow()
{
}

AxisAlignedBB* BlockSnow::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return NULL;
}

bool BlockSnow::isOpaqueCube()
{
	return false;
}

bool BlockSnow::renderAsNormalBlock()
{
	return false;
}

bool BlockSnow::canPlaceBlockAt(World* var1, int var2, int var3, int var4)
{
	int var5 = var1->getBlockId(var2, var3 - 1, var4);
	return var5 != 0 && Block::s_blocksList[var5]->isOpaqueCube() ? var1->getBlockMaterial(var2, var3 - 1, var4)->blocksMovement() : false;
}

void BlockSnow::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	this->canSnowStay(var1, var2, var3, var4);
}

bool BlockSnow::canSnowStay(World* var1, int var2, int var3, int var4)
{
	if(!this->canPlaceBlockAt(var1, var2, var3, var4)) {
	this->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
	var1->setBlockWithNotify(var2, var3, var4, 0);
	return false;
	} else {
	return true;
	}
}

void BlockSnow::harvestBlock(World* var1, int var2, int var3, int var4, int var5)
{
	int var6 = Item::s_snowball->shiftedIndex;
	float var7 = 0.7F;
	double var8 = (double)(var1->rand->nextFloat() * var7) + (double)(1.0F - var7) * 0.5;
	double var10 = (double)(var1->rand->nextFloat() * var7) + (double)(1.0F - var7) * 0.5;
	double var12 = (double)(var1->rand->nextFloat() * var7) + (double)(1.0F - var7) * 0.5;
	EntityItem* var14 = new EntityItem(var1, (double)var2 + var8, (double)var3 + var10, (double)var4 + var12, new ItemStack(var6));
	var14->delayBeforeCanPickup = 10;
	var1->entityJoinedWorld(var14);
	var1->setBlockWithNotify(var2, var3, var4, 0);
}

int BlockSnow::idDropped(int var1, Random* var2)
{
	return Item::s_snowball->shiftedIndex;
}

int BlockSnow::quantityDropped(Random* var1)
{
	return 0;
}

void BlockSnow::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(var1->getSavedLightValue(EnumSkyBlock::Block, var2, var3, var4) > 11) {
	this->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
	var1->setBlockWithNotify(var2, var3, var4, 0);
	}
}

bool BlockSnow::shouldSideBeRendered(IBlockAccess* var1, int var2, int var3, int var4, int var5)
{
	Material* var6 = var1->getBlockMaterial(var2, var3, var4);
	return var5 == 1 ? true : (var6 == this->blockMaterial ? false : Block::shouldSideBeRendered(var1, var2, var3, var4, var5));
}

void BlockSnow::initStatics()
{
}
