#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockSoil.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockSoil.h"
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

BlockSoil::BlockSoil(int var1)
	: Block(var1, Material::s_ground)
{
	this->blockIndexInTexture = 87;
	this->setTickOnLoad(true);
	this->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 15.0F / 16.0F, 1.0F);
	this->setLightOpacity(255);
}

BlockSoil::~BlockSoil()
{
}

AxisAlignedBB* BlockSoil::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return AxisAlignedBB::getBoundingBoxFromPool((double)(var2 + 0), (double)(var3 + 0), (double)(var4 + 0), (double)(var2 + 1), (double)(var3 + 1), (double)(var4 + 1));
}

bool BlockSoil::isOpaqueCube()
{
	return false;
}

bool BlockSoil::renderAsNormalBlock()
{
	return false;
}

int BlockSoil::getBlockTextureFromSideAndMetadata(int var1, int var2)
{
	return var1 == 1 && var2 > 0 ? this->blockIndexInTexture - 1 : (var1 == 1 ? this->blockIndexInTexture : 2);
}

void BlockSoil::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(var5->nextInt(5) == 0) {
	if(this->isWaterNearby(var1, var2, var3, var4)) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, 7);
	} else {
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	if(var6 > 0) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, var6 - 1);
	} else if(!this->isCropsNearby(var1, var2, var3, var4)) {
	var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
	}
	}
	}
}

void BlockSoil::onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5)
{
	if(var1->rand->nextInt(4) == 0) {
	var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
	}
}

bool BlockSoil::isCropsNearby(World* var1, int var2, int var3, int var4)
{
	unsigned char var5 = 0;
	for(int var6 = var2 - var5; var6 <= var2 + var5; ++var6) {
	for(int var7 = var4 - var5; var7 <= var4 + var5; ++var7) {
	if(var1->getBlockId(var6, var3 + 1, var7) == Block::s_crops->blockID) {
	return true;
	}
	}
	}
	return false;
}

bool BlockSoil::isWaterNearby(World* var1, int var2, int var3, int var4)
{
	for(int var5 = var2 - 4; var5 <= var2 + 4; ++var5) {
	for(int var6 = var3; var6 <= var3 + 1; ++var6) {
	for(int var7 = var4 - 4; var7 <= var4 + 4; ++var7) {
	if(var1->getBlockMaterial(var5, var6, var7) == Material::s_water) {
	return true;
	}
	}
	}
	}
	return false;
}

void BlockSoil::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	Block::onNeighborBlockChange(var1, var2, var3, var4, var5);
	Material* var6 = var1->getBlockMaterial(var2, var3 + 1, var4);
	if(var6->isSolid()) {
	var1->setBlockWithNotify(var2, var3, var4, Block::s_dirt->blockID);
	}
}

int BlockSoil::idDropped(int var1, Random* var2)
{
	return Block::s_dirt->idDropped(0, var2);
}

void BlockSoil::initStatics()
{
}
