#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from ChunkProviderClient.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ChunkProviderClient.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockGrass.h"
#include "BlockLeaves.h"
#include "BlockPortal.h"
#include "Chunk.h"
#include "ChunkCoordinates.h"
#include "Class.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityItem.h"
#include "EntityPlayer.h"
#include "Explosion.h"
#include "File.h"
#include "IChunkLoader.h"
#include "IChunkProvider.h"
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

// Pack a ChunkCoordinates (x,z) into the long long key used by chunkMapping. Matches the
// value-equality keying of the original Java Map<ChunkCoordinates, Chunk>.
static inline long long _chunkKey(int x, int z) {
	return ((long long)(unsigned int)x << 32) | (unsigned int)z;
}

ChunkProviderClient::ChunkProviderClient(World* var1)
	: blankChunk(NULL)
	, worldObj(NULL)
{
	this->blankChunk = new Chunk(var1, JArray<unsigned char >(-Short::MIN_VALUE), 0, 0);
	this->blankChunk->isEmptyChunk = true;
	this->blankChunk->neverSave = true;
	this->worldObj = var1;
}

ChunkProviderClient::~ChunkProviderClient()
{
	delete blankChunk;
	blankChunk = NULL;
}

bool ChunkProviderClient::chunkExists(int var1, int var2)
{
	long long var3 = _chunkKey(var1, var2);
	return this->chunkMapping.count(var3);
}

void ChunkProviderClient::unloadChunk(int var1, int var2)
{
	Chunk* var3 = this->provideChunk(var1, var2);
	if(!var3->isEmptyChunk) {
	var3->onChunkUnload();
	}
	this->chunkMapping.erase(_chunkKey(var1, var2));
	this->unusedChunkList.remove(var3);
}

Chunk* ChunkProviderClient::loadChunk(int var1, int var2)
{
	long long var3 = _chunkKey(var1, var2);
	JArray<unsigned char > var4 = JArray<unsigned char >(-Short::MIN_VALUE);
	Chunk* var5 = new Chunk(this->worldObj, var4, var1, var2);
	Arrays::fill(var5->skylightMap->data, (unsigned char)-1);
	this->chunkMapping[var3] = var5;
	var5->isChunkLoaded = true;
	return var5;
}

Chunk* ChunkProviderClient::provideChunk(int var1, int var2)
{
	long long var3 = _chunkKey(var1, var2);
	Chunk* var4 = (Chunk*)this->chunkMapping[var3];
	return var4 == NULL ? this->blankChunk : var4;
}

bool ChunkProviderClient::saveChunks(bool var1, IProgressUpdate* var2)
{
	return true;
}

bool ChunkProviderClient::unloadQueuedChunks()
{
	return false;
}

bool ChunkProviderClient::canSave()
{
	return false;
}

void ChunkProviderClient::populate(IChunkProvider* var1, int var2, int var3)
{
}
