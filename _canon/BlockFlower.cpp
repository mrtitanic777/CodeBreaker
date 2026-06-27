#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockFlower.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockFlower.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
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

BlockFlower::BlockFlower(int var1, int var2)
	: Block(var1, Material::s_plants)
{
	this->blockIndexInTexture = var2;
	this->setTickOnLoad(true);
	float var3 = 0.2F;
	this->setBlockBounds(0.5F - var3, 0.0F, 0.5F - var3, 0.5F + var3, var3 * 3.0F, 0.5F + var3);
}

BlockFlower::~BlockFlower()
{
}

bool BlockFlower::canPlaceBlockAt(World* var1, int var2, int var3, int var4)
{
	return this->canThisPlantGrowOnThisBlockID(var1->getBlockId(var2, var3 - 1, var4));
}

bool BlockFlower::canThisPlantGrowOnThisBlockID(int var1)
{
	return var1 == Block::s_grass->blockID || var1 == Block::s_dirt->blockID || var1 == Block::s_tilledField->blockID;
}

void BlockFlower::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	Block::onNeighborBlockChange(var1, var2, var3, var4, var5);
	this->checkFlowerCanStay(var1, var2, var3, var4);
}

void BlockFlower::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	this->checkFlowerCanStay(var1, var2, var3, var4);
}

void BlockFlower::checkFlowerCanStay(World* var1, int var2, int var3, int var4)
{
	if(!this->canBlockStay(var1, var2, var3, var4)) {
	this->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
	var1->setBlockWithNotify(var2, var3, var4, 0);
	}
}

bool BlockFlower::canBlockStay(World* var1, int var2, int var3, int var4)
{
	return (var1->getBlockLightValue(var2, var3, var4) >= 8 || var1->canBlockSeeTheSky(var2, var3, var4)) && this->canThisPlantGrowOnThisBlockID(var1->getBlockId(var2, var3 - 1, var4));
}

AxisAlignedBB* BlockFlower::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return NULL;
}

bool BlockFlower::isOpaqueCube()
{
	return false;
}

bool BlockFlower::renderAsNormalBlock()
{
	return false;
}

int BlockFlower::getRenderType()
{
	return 1;
}

void BlockFlower::initStatics()
{
}
