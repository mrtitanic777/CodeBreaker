#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ChunkProviderIso.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkProviderIso.h"
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
#include "Exception.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
#include "IOException.h"
#include "IProgressUpdate.h"
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

ChunkProviderIso::ChunkProviderIso(World* var1, IChunkLoader* var2)
	: worldObj(NULL)
	, chunkLoader(NULL)
{
	this->chunks = JArray<Chunk* >(256);
	this->blankBlocks = JArray<unsigned char >(-Short::MIN_VALUE);
	this->worldObj = var1;
	this->chunkLoader = var2;
}

ChunkProviderIso::~ChunkProviderIso()
{
}

bool ChunkProviderIso::chunkExists(int var1, int var2)
{
	int var3 = (var1 & 15) | ((var2 & 15) * 16);
	return this->chunks[var3] != NULL && this->chunks[var3]->isAtLocation(var1, var2);
}

Chunk* ChunkProviderIso::provideChunk(int var1, int var2)
{
	int var3 = (var1 & 15) | ((var2 & 15) * 16);
	try {
	if(!this->chunkExists(var1, var2)) {
	Chunk* var4 = this->func_543_c(var1, var2);
	if(var4 == NULL) {
	var4 = new Chunk(this->worldObj, this->blankBlocks, var1, var2);
	var4->isEmptyChunk = true;
	var4->neverSave = true;
	}
	this->chunks[var3] = var4;
	}
	return this->chunks[var3];
	} catch (Exception* var5) {
	var5->printStackTrace();
	return NULL;
	}
}

Chunk* ChunkProviderIso::func_543_c(int var1, int var2)
{
	try {
	return this->chunkLoader->loadChunk(this->worldObj, var1, var2);
	} catch (IOException* var4) {
	var4->printStackTrace();
	return NULL;
	}
}

void ChunkProviderIso::populate(IChunkProvider* var1, int var2, int var3)
{
}

bool ChunkProviderIso::saveChunks(bool var1, IProgressUpdate* var2)
{
	return true;
}

bool ChunkProviderIso::unloadQueuedChunks()
{
	return false;
}

bool ChunkProviderIso::canSave()
{
	return false;
}

void ChunkProviderIso::initStatics()
{
}
