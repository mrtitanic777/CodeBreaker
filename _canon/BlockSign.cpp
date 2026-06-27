#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockSign.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSign.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockContainer.h"
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
#include "Exception.h"
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
#include "RuntimeException.h"
#include "StepSound.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldChunkManager.h"
#include "WorldProvider.h"

BlockSign::BlockSign(int var1, Class* var2, bool var3)
	: BlockContainer(var1, Material::s_wood)
	, signEntityClass(NULL)
	, isFreestanding(0)
{
	this->isFreestanding = var3;
	this->blockIndexInTexture = 4;
	this->signEntityClass = var2;
	float var4 = 0.25F;
	float var5 = 1.0F;
	this->setBlockBounds(0.5F - var4, 0.0F, 0.5F - var4, 0.5F + var4, var5, 0.5F + var4);
}

BlockSign::~BlockSign()
{
}

AxisAlignedBB* BlockSign::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return NULL;
}

AxisAlignedBB* BlockSign::getSelectedBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	this->setBlockBoundsBasedOnState(var1, var2, var3, var4);
	return BlockContainer::getSelectedBoundingBoxFromPool(var1, var2, var3, var4);
}

void BlockSign::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4)
{
	if(!this->isFreestanding) {
	int var5 = var1->getBlockMetadata(var2, var3, var4);
	float var6 = 9.0F / 32.0F;
	float var7 = 25.0F / 32.0F;
	float var8 = 0.0F;
	float var9 = 1.0F;
	float var10 = 2.0F / 16.0F;
	this->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
	if(var5 == 2) {
	this->setBlockBounds(var8, var6, 1.0F - var10, var9, var7, 1.0F);
	}
	if(var5 == 3) {
	this->setBlockBounds(var8, var6, 0.0F, var9, var7, var10);
	}
	if(var5 == 4) {
	this->setBlockBounds(1.0F - var10, var6, var8, 1.0F, var7, var9);
	}
	if(var5 == 5) {
	this->setBlockBounds(0.0F, var6, var8, var10, var7, var9);
	}
	}
}

int BlockSign::getRenderType()
{
	return -1;
}

bool BlockSign::renderAsNormalBlock()
{
	return false;
}

bool BlockSign::isOpaqueCube()
{
	return false;
}

TileEntity* BlockSign::SetBlockEntity()
{
	try {
	return (TileEntity*)this->signEntityClass->newInstance();
	} catch (Exception* var2) {
	throw new RuntimeException(var2);
	}
}

int BlockSign::idDropped(int var1, Random* var2)
{
	return Item::s_sign->shiftedIndex;
}

void BlockSign::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	bool var6 = false;
	if(this->isFreestanding) {
	if(!var1->getBlockMaterial(var2, var3 - 1, var4)->isSolid()) {
	var6 = true;
	}
	} else {
	int var7 = var1->getBlockMetadata(var2, var3, var4);
	var6 = true;
	if(var7 == 2 && var1->getBlockMaterial(var2, var3, var4 + 1)->isSolid()) {
	var6 = false;
	}
	if(var7 == 3 && var1->getBlockMaterial(var2, var3, var4 - 1)->isSolid()) {
	var6 = false;
	}
	if(var7 == 4 && var1->getBlockMaterial(var2 + 1, var3, var4)->isSolid()) {
	var6 = false;
	}
	if(var7 == 5 && var1->getBlockMaterial(var2 - 1, var3, var4)->isSolid()) {
	var6 = false;
	}
	}
	if(var6) {
	this->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
	var1->setBlockWithNotify(var2, var3, var4, 0);
	}
	BlockContainer::onNeighborBlockChange(var1, var2, var3, var4, var5);
}

void BlockSign::initStatics()
{
}
