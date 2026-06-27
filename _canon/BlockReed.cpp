#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockReed.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockReed.h"
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

BlockReed::BlockReed(int var1, int var2)
	: Block(var1, Material::s_plants)
{
	this->blockIndexInTexture = var2;
	float var3 = 6.0F / 16.0F;
	this->setBlockBounds(0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, 1.0F, 0.5F + var3);
	this->setTickOnLoad(true);
}

BlockReed::~BlockReed()
{
}

void BlockReed::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(var1->getBlockId(var2, var3 + 1, var4) == 0) {
	int var6;
	for(var6 = 1; var1->getBlockId(var2, var3 - var6, var4) == this->blockID; ++var6) {
	}
	if(var6 < 3) {
	int var7 = var1->getBlockMetadata(var2, var3, var4);
	if(var7 == 15) {
	var1->setBlockWithNotify(var2, var3 + 1, var4, this->blockID);
	var1->setBlockMetadataWithNotify(var2, var3, var4, 0);
	} else {
	var1->setBlockMetadataWithNotify(var2, var3, var4, var7 + 1);
	}
	}
	}
}

bool BlockReed::canPlaceBlockAt(World* var1, int var2, int var3, int var4)
{
	int var5 = var1->getBlockId(var2, var3 - 1, var4);
	return var5 == this->blockID ? true : (var5 != Block::s_grass->blockID && var5 != Block::s_dirt->blockID ? false : (var1->getBlockMaterial(var2 - 1, var3 - 1, var4) == Material::s_water ? true : (var1->getBlockMaterial(var2 + 1, var3 - 1, var4) == Material::s_water ? true : (var1->getBlockMaterial(var2, var3 - 1, var4 - 1) == Material::s_water ? true : var1->getBlockMaterial(var2, var3 - 1, var4 + 1) == Material::s_water))));
}

void BlockReed::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	this->checkBlockCoordValid(var1, var2, var3, var4);
}

void BlockReed::checkBlockCoordValid(World* var1, int var2, int var3, int var4)
{
	if(!this->canBlockStay(var1, var2, var3, var4)) {
	this->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
	var1->setBlockWithNotify(var2, var3, var4, 0);
	}
}

bool BlockReed::canBlockStay(World* var1, int var2, int var3, int var4)
{
	return this->canPlaceBlockAt(var1, var2, var3, var4);
}

AxisAlignedBB* BlockReed::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return NULL;
}

int BlockReed::idDropped(int var1, Random* var2)
{
	return Item::s_reed->shiftedIndex;
}

bool BlockReed::isOpaqueCube()
{
	return false;
}

bool BlockReed::renderAsNormalBlock()
{
	return false;
}

int BlockReed::getRenderType()
{
	return 1;
}

void BlockReed::initStatics()
{
}
