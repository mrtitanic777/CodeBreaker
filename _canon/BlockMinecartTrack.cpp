#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockMinecartTrack.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockMinecartTrack.h"
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
#include "MinecartTrackLogic.h"
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

BlockMinecartTrack::BlockMinecartTrack(int var1, int var2)
	: Block(var1, var2, Material::s_circuits)
{
	this->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 2.0F / 16.0F, 1.0F);
}

BlockMinecartTrack::~BlockMinecartTrack()
{
}

AxisAlignedBB* BlockMinecartTrack::getCollisionBoundingBoxFromPool(World* var1, int var2, int var3, int var4)
{
	return NULL;
}

bool BlockMinecartTrack::isOpaqueCube()
{
	return false;
}

MovingObjectPosition* BlockMinecartTrack::collisionRayTrace(World* var1, int var2, int var3, int var4, Vec3D* var5, Vec3D* var6)
{
	this->setBlockBoundsBasedOnState(var1, var2, var3, var4);
	return Block::collisionRayTrace(var1, var2, var3, var4, var5, var6);
}

void BlockMinecartTrack::setBlockBoundsBasedOnState(IBlockAccess* var1, int var2, int var3, int var4)
{
	int var5 = var1->getBlockMetadata(var2, var3, var4);
	if(var5 >= 2 && var5 <= 5) {
	this->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 10.0F / 16.0F, 1.0F);
	} else {
	this->setBlockBounds(0.0F, 0.0F, 0.0F, 1.0F, 2.0F / 16.0F, 1.0F);
	}
}

int BlockMinecartTrack::getBlockTextureFromSideAndMetadata(int var1, int var2)
{
	return var2 >= 6 ? this->blockIndexInTexture - 16 : this->blockIndexInTexture;
}

bool BlockMinecartTrack::renderAsNormalBlock()
{
	return false;
}

int BlockMinecartTrack::getRenderType()
{
	return 9;
}

int BlockMinecartTrack::quantityDropped(Random* var1)
{
	return 1;
}

bool BlockMinecartTrack::canPlaceBlockAt(World* var1, int var2, int var3, int var4)
{
	return var1->isBlockOpaqueCube(var2, var3 - 1, var4);
}

void BlockMinecartTrack::onBlockAdded(World* var1, int var2, int var3, int var4)
{
	if(!var1->multiplayerWorld) {
	var1->setBlockMetadataWithNotify(var2, var3, var4, 15);
	this->refreshTrackShape(var1, var2, var3, var4);
	}
}

void BlockMinecartTrack::onNeighborBlockChange(World* var1, int var2, int var3, int var4, int var5)
{
	if(!var1->multiplayerWorld) {
	int var6 = var1->getBlockMetadata(var2, var3, var4);
	bool var7 = false;
	if(!var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
	var7 = true;
	}
	if(var6 == 2 && !var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
	var7 = true;
	}
	if(var6 == 3 && !var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
	var7 = true;
	}
	if(var6 == 4 && !var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
	var7 = true;
	}
	if(var6 == 5 && !var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
	var7 = true;
	}
	if(var7) {
	this->dropBlockAsItem(var1, var2, var3, var4, var1->getBlockMetadata(var2, var3, var4));
	var1->setBlockWithNotify(var2, var3, var4, 0);
	} else if(var5 > 0 && Block::s_blocksList[var5]->canProvidePower() && MinecartTrackLogic::getNumberOfAdjacentTracks(new MinecartTrackLogic(this, var1, var2, var3, var4)) == 3) {
	this->refreshTrackShape(var1, var2, var3, var4);
	}
	}
}

void BlockMinecartTrack::refreshTrackShape(World* var1, int var2, int var3, int var4)
{
	if(!var1->multiplayerWorld) {
	(new MinecartTrackLogic(this, var1, var2, var3, var4))->refreshTrackShape(var1->isBlockIndirectlyGettingPowered(var2, var3, var4));
	}
}

void BlockMinecartTrack::initStatics()
{
}
