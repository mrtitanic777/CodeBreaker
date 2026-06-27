#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ChunkCache.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkCache.h"
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

ChunkCache::ChunkCache(World* var1, int var2, int var3, int var4, int var5, int var6, int var7)
	: chunkX(0)
	, chunkZ(0)
	, worldObj(NULL)
{
	this->worldObj = var1;
	this->chunkX = var2 >> 4;
	this->chunkZ = var4 >> 4;
	int var8 = var5 >> 4;
	int var9 = var7 >> 4;
	// Java: chunkArray = new Chunk[var8 - chunkX + 1][var9 - chunkZ + 1]; a jagged 2D array of
	// Chunk*. C++03 cannot new[] with runtime dims, so build the JArray<JArray<Chunk*>> by hand:
	// allocate the outer rows, then each inner column row.
	this->chunkArray = JArray<JArray<Chunk* > >(var8 - this->chunkX + 1);
	for(int var12 = 0; var12 < this->chunkArray.length; ++var12) {
	this->chunkArray[var12] = JArray<Chunk* >(var9 - this->chunkZ + 1);
	}
	for(int var10 = this->chunkX; var10 <= var8; ++var10) {
	for(int var11 = this->chunkZ; var11 <= var9; ++var11) {
	this->chunkArray[var10 - this->chunkX][var11 - this->chunkZ] = var1->getChunkFromChunkCoords(var10, var11);
	}
	}
}

ChunkCache::~ChunkCache()
{
}

int ChunkCache::getBlockId(int var1, int var2, int var3)
{
	if(var2 < 0) {
	return 0;
	} else if(var2 >= 128) {
	return 0;
	} else {
	int var4 = (var1 >> 4) - this->chunkX;
	int var5 = (var3 >> 4) - this->chunkZ;
	return this->chunkArray[var4][var5]->getBlockID(var1 & 15, var2, var3 & 15);
	}
}

TileEntity* ChunkCache::getBlockTileEntity(int var1, int var2, int var3)
{
	int var4 = (var1 >> 4) - this->chunkX;
	int var5 = (var3 >> 4) - this->chunkZ;
	return this->chunkArray[var4][var5]->getChunkBlockTileEntity(var1 & 15, var2, var3 & 15);
}

float ChunkCache::getLightBrightness(int var1, int var2, int var3)
{
	return this->worldObj->worldProvider->lightBrightnessTable[this->getBlockLightValue(var1, var2, var3)];
}

int ChunkCache::getBlockLightValue(int var1, int var2, int var3)
{
	return this->addPendingConnection(var1, var2, var3, true);
}

int ChunkCache::addPendingConnection(int var1, int var2, int var3, bool var4)
{
	if(var1 >= -32000000 && var3 >= -32000000 && var1 < 32000000 && var3 <= 32000000) {
	int var5;
	int var6;
	if(var4) {
	var5 = this->getBlockId(var1, var2, var3);
	if(var5 == Block::s_stairSingle->blockID || var5 == Block::s_tilledField->blockID) {
	var6 = this->addPendingConnection(var1, var2 + 1, var3, false);
	int var7 = this->addPendingConnection(var1 + 1, var2, var3, false);
	int var8 = this->addPendingConnection(var1 - 1, var2, var3, false);
	int var9 = this->addPendingConnection(var1, var2, var3 + 1, false);
	int var10 = this->addPendingConnection(var1, var2, var3 - 1, false);
	if(var7 > var6) {
	var6 = var7;
	}
	if(var8 > var6) {
	var6 = var8;
	}
	if(var9 > var6) {
	var6 = var9;
	}
	if(var10 > var6) {
	var6 = var10;
	}
	return var6;
	}
	}
	if(var2 < 0) {
	return 0;
	} else if(var2 >= 128) {
	var5 = 15 - this->worldObj->skylightSubtracted;
	if(var5 < 0) {
	var5 = 0;
	}
	return var5;
	} else {
	var5 = (var1 >> 4) - this->chunkX;
	var6 = (var3 >> 4) - this->chunkZ;
	return this->chunkArray[var5][var6]->getBlockLightValue(var1 & 15, var2, var3 & 15, this->worldObj->skylightSubtracted);
	}
	} else {
	return 15;
	}
}

int ChunkCache::getBlockMetadata(int var1, int var2, int var3)
{
	if(var2 < 0) {
	return 0;
	} else if(var2 >= 128) {
	return 0;
	} else {
	int var4 = (var1 >> 4) - this->chunkX;
	int var5 = (var3 >> 4) - this->chunkZ;
	return this->chunkArray[var4][var5]->getBlockMetadata(var1 & 15, var2, var3 & 15);
	}
}

Material* ChunkCache::getBlockMaterial(int var1, int var2, int var3)
{
	int var4 = this->getBlockId(var1, var2, var3);
	return var4 == 0 ? Material::s_air : Block::s_blocksList[var4]->blockMaterial;
}

bool ChunkCache::isBlockOpaqueCube(int var1, int var2, int var3)
{
	Block* var4 = Block::s_blocksList[this->getBlockId(var1, var2, var3)];
	return var4 == NULL ? false : var4->isOpaqueCube();
}

WorldChunkManager* ChunkCache::getWorldChunkManager()
{
	return this->worldObj->getWorldChunkManager();
}
