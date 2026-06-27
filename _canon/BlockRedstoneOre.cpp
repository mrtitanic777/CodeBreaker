#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from BlockRedstoneOre.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "BlockRedstoneOre.h"
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

BlockRedstoneOre::BlockRedstoneOre(int var1, int var2, bool var3)
	: Block(var1, var2, Material::s_rock)
	, glowing(0)
{
	if(var3) {
	this->setTickOnLoad(true);
	}
	this->glowing = var3;
}

BlockRedstoneOre::~BlockRedstoneOre()
{
}

int BlockRedstoneOre::tickRate()
{
	return 30;
}

void BlockRedstoneOre::onBlockClicked(World* var1, int var2, int var3, int var4, EntityPlayer* var5)
{
	this->glow(var1, var2, var3, var4);
	Block::onBlockClicked(var1, var2, var3, var4, var5);
}

void BlockRedstoneOre::onEntityWalking(World* var1, int var2, int var3, int var4, Entity* var5)
{
	this->glow(var1, var2, var3, var4);
	Block::onEntityWalking(var1, var2, var3, var4, var5);
}

bool BlockRedstoneOre::blockActivated(World* var1, int var2, int var3, int var4, EntityPlayer* var5)
{
	this->glow(var1, var2, var3, var4);
	return Block::blockActivated(var1, var2, var3, var4, var5);
}

void BlockRedstoneOre::glow(World* var1, int var2, int var3, int var4)
{
	this->sparkle(var1, var2, var3, var4);
	if(this->blockID == Block::s_oreRedstone->blockID) {
	var1->setBlockWithNotify(var2, var3, var4, Block::s_oreRedstoneGlowing->blockID);
	}
}

void BlockRedstoneOre::updateTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(this->blockID == Block::s_oreRedstoneGlowing->blockID) {
	var1->setBlockWithNotify(var2, var3, var4, Block::s_oreRedstone->blockID);
	}
}

int BlockRedstoneOre::idDropped(int var1, Random* var2)
{
	return Item::s_redstone->shiftedIndex;
}

int BlockRedstoneOre::quantityDropped(Random* var1)
{
	return 4 + var1->nextInt(2);
}

void BlockRedstoneOre::randomDisplayTick(World* var1, int var2, int var3, int var4, Random* var5)
{
	if(this->glowing) {
	this->sparkle(var1, var2, var3, var4);
	}
}

void BlockRedstoneOre::sparkle(World* var1, int var2, int var3, int var4)
{
	Random* var5 = var1->rand;
	double var6 = 1.0 / 16.0;
	for(int var8 = 0; var8 < 6; ++var8) {
	double var9 = (double)((float)var2 + var5->nextFloat());
	double var11 = (double)((float)var3 + var5->nextFloat());
	double var13 = (double)((float)var4 + var5->nextFloat());
	if(var8 == 0 && !var1->isBlockOpaqueCube(var2, var3 + 1, var4)) {
	var11 = (double)(var3 + 1) + var6;
	}
	if(var8 == 1 && !var1->isBlockOpaqueCube(var2, var3 - 1, var4)) {
	var11 = (double)(var3 + 0) - var6;
	}
	if(var8 == 2 && !var1->isBlockOpaqueCube(var2, var3, var4 + 1)) {
	var13 = (double)(var4 + 1) + var6;
	}
	if(var8 == 3 && !var1->isBlockOpaqueCube(var2, var3, var4 - 1)) {
	var13 = (double)(var4 + 0) - var6;
	}
	if(var8 == 4 && !var1->isBlockOpaqueCube(var2 + 1, var3, var4)) {
	var9 = (double)(var2 + 1) + var6;
	}
	if(var8 == 5 && !var1->isBlockOpaqueCube(var2 - 1, var3, var4)) {
	var9 = (double)(var2 + 0) - var6;
	}
	if(var9 < (double)var2 || var9 > (double)(var2 + 1) || var11 < 0.0 || var11 > (double)(var3 + 1) || var13 < (double)var4 || var13 > (double)(var4 + 1)) {
	var1->spawnParticle("reddust", var9, var11, var13, 0.0, 0.0, 0.0);
	}
	}
}

void BlockRedstoneOre::initStatics()
{
}
